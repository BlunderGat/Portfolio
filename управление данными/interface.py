from tabulate import tabulate
from typing import Tuple, List
from queries import FootballQueries

class UserInterface:
    def __init__(self, queries: FootballQueries):
        self.queries = queries
    @staticmethod
    def print_header(title: str):
        print(f"{title:^60}")

    @staticmethod
    def print_table(columns: List[str], data: List[Tuple]):
        if not data:
            print("Данные не найдены")
            return
        print(tabulate(data, headers=columns, tablefmt="grid", numalign="center"))
        print(f"Найдено записей: {len(data)}")
    def show_reports_menu(self):
        while True:
            self.print_header("СПРАВКИ И ЗАПРОСЫ")
            print("1. Матчи команды")
            print("2. Игроки матча")
            print("3. Результативность игрока")
            print("4. Цена билета на матч")
            print("5. Назад")
            choice = input("Выберите действие (1-5): ")
            if choice == "1":
                self.show_team_matches()
            elif choice == "2":
                self.show_match_players()
            elif choice == "3":
                self.show_player_performance()
            elif choice == "4":
                self.show_ticket_price()
            elif choice == "5":
                break
            else:
                print("Неверный выбор")
    def show_team_matches(self):
        team = input("Введите название команды: ").strip()
        if not team:
            print("Название команды не может быть пустым")
            return
        columns, data = self.queries.get_team_matches(team)
        self.print_table(columns, data)
    def show_match_players(self):
        print("Поиск игроков матча:")
        team = input("Название команды: ")
        city = input("Город команды: ")
        date = input("Дата матча (ГГГГ-ММ-ДД): ")
        columns, data = self.queries.get_match_players(team, city, date)
        self.print_table(columns, data)
    def show_player_performance(self):
        print("Результативность игрока:")
        team = input("Команда: ")
        city = input("Город: ")
        date = input("Дата матча: ")
        player = input("ФИО игрока: ")
        columns, data = self.queries.get_player_performance(team, city, date, player)
        self.print_table(columns, data)
    def show_ticket_price(self):
        print("Расчет цены билета:")
        team1 = input("Первая команда: ")
        team2 = input("Вторая команда: ")
        price = self.queries.calculate_ticket_price(team1, team2)
        if price:
            print(f"Примерная цена билета: {price:.2f} руб.")
        else:
            print("Матч между этими командами не найден")
    def show_changes_menu(self):
        while True:
            self.print_header("ИЗМЕНЕНИЯ ДАННЫХ")
            print("1. Переход игрока")
            print("2. Отмена встречи")
            print("3. Назначение тренера")
            print("4. Назад")
            choice = input("Выберите действие (1-4): ")
            if choice == "1":
                self.transfer_player()
            elif choice == "2":
                self.cancel_match()
            elif choice == "3":
                self.change_coach()
            elif choice == "4":
                break
            else:
                print("Неверный выбор")
    def transfer_player(self):
        print("Перевод игрока в другую команду:")
        player = input("ФИО игрока: ")
        new_team = input("Новая команда: ")
        result = self.queries.transfer_player(player, new_team)
        print(f"{result}")
    def cancel_match(self):
        print("Отмена матча:")
        date = input("Дата матча (ГГГГ-ММ-ДД): ")
        team1 = input("Первая команда: ")
        team2 = input("Вторая команда: ")
        result = self.queries.cancel_match(date, team1, team2)
        print(f"{result}")
    def change_coach(self):
        print("Назначение нового тренера:")
        team = input("  Команда: ")
        coach = input("  ФИО нового тренера: ")
        result = self.queries.change_coach(team, coach)
        print(f"{result}")
    def show_reports_menu_advanced(self):
        while True:
            self.print_header("ОТЧЕТЫ И АНАЛИТИКА")
            print("1. Игры на стадионе")
            print("2. Отчет за период")
            print("3. Список команд")
            print("4. Список игроков")
            print("5. Назад")
            choice = input("Выберите действие (1-5): ")
            if choice == "1":
                self.show_stadium_games()
            elif choice == "2":
                self.show_season_report()
            elif choice == "3":
                self.show_all_teams()
            elif choice == "4":
                self.show_all_players()
            elif choice == "5":
                break
            else:
                print("Неверный выбор")
    def show_stadium_games(self):
        stadium = input("Введите название стадиона: ")
        columns, data = self.queries.get_stadium_games(stadium)
        self.print_table(columns, data)
    def show_season_report(self):
        print("Отчет за период:")
        start = input("  Начало периода (ГГГГ-ММ-ДД): ")
        end = input("  Конец периода (ГГГГ-ММ-ДД): ")
        report = self.queries.get_season_report(start, end)
        if report['stats']:
            stats = report['stats']
            print("ОБЩАЯ СТАТИСТИКА:")
            print(f"   Всего матчей: {stats[0]}")
            print(f"   Побед хозяев: {stats[1]}")
            print(f"   Побед гостей: {stats[2]}")
            print(f"   Ничьих: {stats[3]}")
            print(f"   Всего голов: {stats[4]}")
        if report['scorers']:
            print("БОМБАРДИРЫ:")
            for player in report['scorers']:
                print(f"   {player[0]} ({player[1]}) - {player[2]} голов в {player[3]} матчах")
    def show_all_teams(self):
        teams = self.queries.get_all_teams()
        print("СПИСОК КОМАНД:")
        for team in teams:
            print(f"   {team[1]} ({team[2]}) - ID: {team[0]}")
    def show_all_players(self):
        players = self.queries.get_all_players()
        print("СПИСОК ИГРОКОВ:")
        for player in players:
            team = player[1] if player[1] else "Без команды"
            print(f"   {player[0]} - {team}")
    def run(self):
        while True:
            self.print_header("СИСТЕМА АДМИНИСТРАТОРА ФУТБОЛЬНОЙ КОМАНДЫ")
            print("1. Справки и запросы")
            print("2. Изменения данных")
            print("3. Отчеты и аналитика")
            print("4. Выход")
            choice = input("Выберите раздел (1-4): ")
            if choice == "1":
                self.show_reports_menu()
            elif choice == "2":
                self.show_changes_menu()
            elif choice == "3":
                self.show_reports_menu_advanced()
            elif choice == "4":
                break
            else:
                print("Неверный выбор")
            if choice != "4":
                input("Нажмите Enter для продолжения")