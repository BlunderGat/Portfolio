import sqlite3
from typing import List, Tuple, Optional
from entities import *


class DatabaseManager:
    # Управление подключением и созданием БД
    def __init__(self, db_name: str = 'football.db'):
        self.db_name = db_name
        self.conn = None
        self.cursor = None

    def connect(self):
        # Установить соединение с БД
        self.conn = sqlite3.connect(self.db_name)
        self.cursor = self.conn.cursor()
        self.cursor.execute("PRAGMA foreign_keys = ON")
        return self

    def disconnect(self):
        # Закрыть соединение с БД
        if self.conn:
            self.conn.close()

    def __enter__(self):
        # Контекстный менеджер
        self.connect()
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        # Контекстный менеджер
        self.disconnect()

    def execute_query(self, query: str, params: tuple = ()) -> sqlite3.Cursor:
        # Выполнить SQL-запрос
        return self.cursor.execute(query, params)

    def commit(self):
        # Подтвердить изменения
        self.conn.commit()

    def create_tables(self):
        # Создать таблицы базы данных
        tables = [
            # Команд
            """
            CREATE TABLE IF NOT EXISTS teams (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                name TEXT NOT NULL UNIQUE,
                city TEXT NOT NULL,
                coach_name TEXT NOT NULL,
                last_season_position INTEGER CHECK(last_season_position > 0)
            )
            """,

            # Игроки
            """
            CREATE TABLE IF NOT EXISTS players (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                full_name TEXT NOT NULL,
                current_team_id INTEGER,
                FOREIGN KEY (current_team_id) REFERENCES teams(id) ON DELETE SET NULL
            )
            """,

            # Стадионы
            """
            CREATE TABLE IF NOT EXISTS stadiums (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                name TEXT NOT NULL UNIQUE,
                city TEXT NOT NULL,
                capacity INTEGER CHECK(capacity > 0)
            )
            """,

            # Матчи
            """
            CREATE TABLE IF NOT EXISTS matches (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                match_date DATE NOT NULL,
                stadium_id INTEGER NOT NULL,
                home_team_id INTEGER NOT NULL,
                away_team_id INTEGER NOT NULL,
                home_score INTEGER DEFAULT 0,
                away_score INTEGER DEFAULT 0,
                status TEXT DEFAULT 'запланирован' 
                CHECK(status IN ('запланирован', 'состоялся', 'отменен', 'перенесен')),
                FOREIGN KEY (stadium_id) REFERENCES stadiums(id),
                FOREIGN KEY (home_team_id) REFERENCES teams(id),
                FOREIGN KEY (away_team_id) REFERENCES teams(id),
                UNIQUE(home_team_id, match_date),
                UNIQUE(away_team_id, match_date)
            )
            """,

            # Выступления игроков
            """
            CREATE TABLE IF NOT EXISTS player_performance (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                player_id INTEGER NOT NULL,
                match_id INTEGER NOT NULL,
                team_id INTEGER NOT NULL,
                player_number INTEGER,
                goals_scored INTEGER DEFAULT 0,
                FOREIGN KEY (player_id) REFERENCES players(id),
                FOREIGN KEY (match_id) REFERENCES matches(id),
                FOREIGN KEY (team_id) REFERENCES teams(id),
                UNIQUE(player_id, match_id)
            )
            """,

            # Переходы игроков
            """
            CREATE TABLE IF NOT EXISTS player_transfers (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                player_id INTEGER NOT NULL,
                old_team_id INTEGER,
                new_team_id INTEGER,
                transfer_date DATE DEFAULT CURRENT_DATE,
                FOREIGN KEY (player_id) REFERENCES players(id),
                FOREIGN KEY (old_team_id) REFERENCES teams(id),
                FOREIGN KEY (new_team_id) REFERENCES teams(id)
            )
            """
        ]

        for table_sql in tables:
            self.execute_query(table_sql)
        self.commit()

    def add_sample_data(self):
        # Добавить тестовые данные
        sample_data = [
            # Команды
            ("INSERT OR IGNORE INTO teams (name, city, coach_name, last_season_position) VALUES "
             "('Спартак', 'Москва', 'Иванов И.И.', 1),"
             "('Зенит', 'Санкт-Петербург', 'Петров П.П.', 2),"
             "('Локомотив', 'Москва', 'Сидоров С.С.', 3),"
             "('Ростов', 'Ростов-на-Дону', 'Кузнецов К.К.', 10)"),

            # Стадионы
            ("INSERT OR IGNORE INTO stadiums (name, city, capacity) VALUES "
             "('Открытие Банк Арена', 'Москва', 45360),"
             "('Газпром Арена', 'Санкт-Петербург', 45360),"
             "('Ростов Арена', 'Ростов-на-Дону', 45000)"),

            # Игроки
            ("INSERT OR IGNORE INTO players (full_name, current_team_id) VALUES "
             "('Смолов Ф.К.', 1),"
             "('Дзюба А.А.', 2),"
             "('Мирзов И.И.', 1),"
             "('Промес К.К.', 3),"
             "('Иванов А.А.', 4),"
             "('Петров Б.Б.', 4)")
        ]

        for sql in sample_data:
            try:
                self.execute_query(sql)
            except Exception as e:
                print(f"Ошибка при добавлении данных: {e}")

        self.commit()

        # Добавить тестовые матчи
        self.add_sample_matches()

    def add_sample_matches(self):
        """Добавить тестовые матчи и выступления игроков"""
        try:
            # Добавить матчи
            matches_sql = """
            INSERT OR IGNORE INTO matches (match_date, stadium_id, home_team_id, away_team_id, home_score, away_score, status) VALUES
            ('2024-01-15', 1, 1, 2, 2, 1, 'состоялся'),
            ('2024-01-20', 2, 2, 3, 1, 1, 'состоялся'),
            ('2024-01-25', 1, 3, 4, 3, 0, 'состоялся'),
            ('2024-02-01', 3, 4, 1, 1, 2, 'состоялся'),
            ('2024-02-10', 1, 1, 3, 0, 0, 'запланирован'),
            ('2024-02-15', 2, 2, 4, 0, 0, 'запланирован')
            """

            self.execute_query(matches_sql)

            # Добавить выступления игроков в матчах
            performances_sql = """
            INSERT OR IGNORE INTO player_performance (player_id, match_id, team_id, player_number, goals_scored) VALUES
            -- Матч 1: Спартак vs Зенит (2:1)
            (1, 1, 1, 10, 1),  -- Смолов забил 1 гол
            (3, 1, 1, 7, 1),   -- Мирзов забил 1 гол
            (2, 1, 2, 22, 1),  -- Дзюба забил 1 гол

            -- Матч 2: Зенит vs Локомотив (1:1)
            (2, 2, 2, 22, 1),  -- Дзюба забил 1 гол
            (4, 2, 3, 11, 1),  -- Промес забил 1 гол

            -- Матч 3: Локомотив vs Ростов (3:0)
            (4, 3, 3, 11, 2),  -- Промес забил 2 гола
            (5, 3, 4, 1, 0),   -- Иванов не забил

            -- Матч 4: Ростов vs Спартак (1:2)
            (5, 4, 4, 1, 1),   -- Иванов забил 1 гол
            (1, 4, 1, 10, 1),  -- Смолов забил 1 гол
            (3, 4, 1, 7, 1)    -- Мирзов забил 1 гол
            """

            self.execute_query(performances_sql)
            self.commit()
            print("Тестовые матчи добавлены")

        except Exception as e:
            print(f"Ошибка при добавлении матчей: {e}")