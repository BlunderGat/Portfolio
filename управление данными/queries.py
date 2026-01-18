from typing import List, Tuple, Optional, Dict, Any
from datetime import date
from database import DatabaseManager
from entities import *

class FootballQueries:
    def __init__(self, db_manager: DatabaseManager):
        self.db = db_manager
    def get_team_matches(self, team_name: str) -> Tuple[List[str], List[Tuple]]:
        # Получить все матчи команды
        query = """
        SELECT 
            m.match_date,
            t1.name as 'Наша команда',
            t2.name as 'Противник',
            m.home_score,
            m.away_score,
            m.status
        FROM matches m
        JOIN teams t1 ON m.home_team_id = t1.id
        JOIN teams t2 ON m.away_team_id = t2.id
        WHERE t1.name = ? OR t2.name = ?
        ORDER BY m.match_date
        """
        cursor = self.db.execute_query(query, (team_name, team_name))
        columns = [desc[0] for desc in cursor.description]
        return columns, cursor.fetchall()

    def get_match_players(self, team_name: str, city: str, match_date: str) -> Tuple[List[str], List[Tuple]]:
        # Получить игроков, участвовавших в матче
        query = """
        SELECT 
            p.full_name as 'ФИО',
            pp.player_number as 'Номер',
            pp.goals_scored as 'Голы',
            t.name as 'Команда'
        FROM player_performance pp
        JOIN players p ON pp.player_id = p.id
        JOIN matches m ON pp.match_id = m.id
        JOIN teams t ON pp.team_id = t.id
        WHERE t.name = ? AND t.city = ? AND DATE(m.match_date) = DATE(?)
        ORDER BY pp.player_number
        """
        cursor = self.db.execute_query(query, (team_name, city, match_date))
        columns = [desc[0] for desc in cursor.description]
        return columns, cursor.fetchall()
    def get_player_performance(self, team_name: str, city: str,
                               match_date: str, player_name: str) -> Tuple[List[str], List[Tuple]]:
        # Получить результативность игрока в матче
        query = """
        SELECT 
            p.full_name as 'ФИО',
            pp.player_number as 'Номер',
            pp.goals_scored as 'Забито голов',
            t.name as 'Команда',
            m.match_date as 'Дата матча'
        FROM player_performance pp
        JOIN players p ON pp.player_id = p.id
        JOIN matches m ON pp.match_id = m.id
        JOIN teams t ON pp.team_id = t.id
        WHERE t.name = ? AND t.city = ? 
          AND DATE(m.match_date) = DATE(?) 
          AND p.full_name LIKE ?
        """
        cursor = self.db.execute_query(query,
                                       (team_name, city, match_date, f"%{player_name}%"))
        columns = [desc[0] for desc in cursor.description]
        return columns, cursor.fetchall()
    def calculate_ticket_price(self, team1_name: str, team2_name: str) -> Optional[float]:
        # Рассчитать цену билета на матч
        query = """
        SELECT 
            t1.last_season_position as pos1,
            t2.last_season_position as pos2,
            s.capacity
        FROM teams t1
        CROSS JOIN teams t2
        LEFT JOIN matches m ON (m.home_team_id = t1.id AND m.away_team_id = t2.id) 
                            OR (m.home_team_id = t2.id AND m.away_team_id = t1.id)
        LEFT JOIN stadiums s ON m.stadium_id = s.id
        WHERE t1.name = ? AND t2.name = ?
        LIMIT 1
        """

        cursor = self.db.execute_query(query, (team1_name, team2_name))
        result = cursor.fetchone()
        if result and result[2]:  # Проверяем наличие capacity
            pos1, pos2, capacity = result
            # Формула расчета цены
            base_price = 500
            # Коэффициент за положение команд
            position_bonus = (20 - (pos1 + pos2)) * 50
            # Коэффициент за вместимость
            capacity_bonus = capacity / 10000 * 100
            price = base_price + position_bonus + capacity_bonus
            return max(200, round(price, 2))  # Минимальная цена
        return None

    def transfer_player(self, player_name: str, new_team_name: str) -> str:
        # Перевести игрока в другую команду
        try:
            # Найти игрока
            cursor = self.db.execute_query(
                "SELECT id, current_team_id FROM players WHERE full_name = ?",
                (player_name,)
            )
            player = cursor.fetchone()

            if not player:
                return "Игрок не найден"

            player_id, old_team_id = player

            # Найти новую команду
            cursor = self.db.execute_query(
                "SELECT id FROM teams WHERE name = ?",
                (new_team_name,)
            )
            new_team = cursor.fetchone()

            if not new_team:
                return "Команда не найдена"

            new_team_id = new_team[0]

            # Записать переход в историю
            self.db.execute_query(
                "INSERT INTO player_transfers (player_id, old_team_id, new_team_id) VALUES (?, ?, ?)",
                (player_id, old_team_id, new_team_id)
            )

            # Обновить текущую команду игрока
            self.db.execute_query(
                "UPDATE players SET current_team_id = ? WHERE id = ?",
                (new_team_id, player_id)
            )

            self.db.commit()
            return f"Игрок {player_name} переведен в команду {new_team_name}"

        except Exception as e:
            return f"Ошибка: {str(e)}"

    def cancel_match(self, match_date: str, team1_name: str, team2_name: str) -> str:
        # Отменить матч
        try:
            query = """
            UPDATE matches 
            SET status = 'отменен' 
            WHERE DATE(match_date) = DATE(?)
              AND ((home_team_id = (SELECT id FROM teams WHERE name = ?) 
                    AND away_team_id = (SELECT id FROM teams WHERE name = ?))
                   OR (home_team_id = (SELECT id FROM teams WHERE name = ?) 
                       AND away_team_id = (SELECT id FROM teams WHERE name = ?)))
            """
            self.db.execute_query(query,
                                  (match_date, team1_name, team2_name, team2_name, team1_name))
            rows = self.db.cursor.rowcount

            if rows > 0:
                self.db.commit()
                return f"Матч между {team1_name} и {team2_name} отменен"
            else:
                return "Матч не найден"

        except Exception as e:
            return f"Ошибка: {str(e)}"

    def change_coach(self, team_name: str, new_coach: str) -> str:
        # Назначить нового тренера
        try:
            self.db.execute_query(
                "UPDATE teams SET coach_name = ? WHERE name = ?",
                (new_coach, team_name)
            )

            if self.db.cursor.rowcount > 0:
                self.db.commit()
                return f"Тренер команды '{team_name}' изменен на '{new_coach}'"
            else:
                return f"Команда '{team_name}' не найдена"

        except Exception as e:
            return f"Ошибка: {str(e)}"

    def get_stadium_games(self, stadium_name: str) -> Tuple[List[str], List[Tuple]]:
        # Получить игры на стадионе
        query = """
        SELECT 
            m.match_date as 'Дата',
            t1.name as 'Хозяева',
            t2.name as 'Гости',
            m.home_score as 'Голы хозяев',
            m.away_score as 'Голы гостей',
            CASE 
                WHEN m.home_score > m.away_score THEN 'Победа хозяев'
                WHEN m.home_score < m.away_score THEN 'Победа гостей'
                ELSE 'Ничья'
            END as 'Результат',
            m.status as 'Статус'
        FROM matches m
        JOIN stadiums s ON m.stadium_id = s.id
        JOIN teams t1 ON m.home_team_id = t1.id
        JOIN teams t2 ON m.away_team_id = t2.id
        WHERE s.name = ?
        ORDER BY m.match_date DESC
        """

        cursor = self.db.execute_query(query, (stadium_name,))
        columns = [desc[0] for desc in cursor.description]
        return columns, cursor.fetchall()
    def get_season_report(self, start_date: str, end_date: str) -> Dict[str, Any]:
        # Сформировать отчет за период
        # Общая статистика
        stats_query = """
        SELECT 
            COUNT(*) as total_matches,
            SUM(CASE WHEN home_score > away_score THEN 1 ELSE 0 END) as home_wins,
            SUM(CASE WHEN away_score > home_score THEN 1 ELSE 0 END) as away_wins,
            SUM(CASE WHEN home_score = away_score THEN 1 ELSE 0 END) as draws,
            SUM(home_score + away_score) as total_goals
        FROM matches 
        WHERE status = 'состоялся' 
          AND DATE(match_date) BETWEEN DATE(?) AND DATE(?)
        """
        cursor = self.db.execute_query(stats_query, (start_date, end_date))
        stats = cursor.fetchone()

        # Бомбардиры
        scorers_query = """
        SELECT 
            p.full_name as player_name,
            t.name as team_name,
            SUM(pp.goals_scored) as total_goals,
            COUNT(DISTINCT m.id) as matches_played
        FROM player_performance pp
        JOIN players p ON pp.player_id = p.id
        JOIN teams t ON pp.team_id = t.id
        JOIN matches m ON pp.match_id = m.id
        WHERE pp.goals_scored > 0 
          AND DATE(m.match_date) BETWEEN DATE(?) AND DATE(?)
        GROUP BY p.id, t.id
        HAVING total_goals > 0
        ORDER BY total_goals DESC, player_name
        """
        cursor = self.db.execute_query(scorers_query, (start_date, end_date))
        scorers = cursor.fetchall()

        # Детали матчей
        matches_query = """
        SELECT 
            m.match_date,
            t1.name as home_team,
            t2.name as away_team,
            m.home_score,
            m.away_score,
            s.name as stadium
        FROM matches m
        JOIN teams t1 ON m.home_team_id = t1.id
        JOIN teams t2 ON m.away_team_id = t2.id
        JOIN stadiums s ON m.stadium_id = s.id
        WHERE m.status = 'состоялся'
          AND DATE(m.match_date) BETWEEN DATE(?) AND DATE(?)
        ORDER BY m.match_date
        """
        cursor = self.db.execute_query(matches_query, (start_date, end_date))
        matches = cursor.fetchall()
        return {
            'stats': stats,
            'scorers': scorers,
            'matches': matches
        }
    def get_all_teams(self) -> List[Tuple]:
        # Получить список всех команд
        cursor = self.db.execute_query("SELECT id, name, city FROM teams ORDER BY name")
        return cursor.fetchall()
    def get_all_players(self) -> List[Tuple]:
        # Получить список всех игроков
        cursor = self.db.execute_query(
            "SELECT p.full_name, t.name as team_name FROM players p "
            "LEFT JOIN teams t ON p.current_team_id = t.id ORDER BY p.full_name"
        )
        return cursor.fetchall()
    def get_all_stadiums(self) -> List[Tuple]:
        # Получить список всех стадионов
        cursor = self.db.execute_query("SELECT name, city, capacity FROM stadiums ORDER BY name")
        return cursor.fetchall()