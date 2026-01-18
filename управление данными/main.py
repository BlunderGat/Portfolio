from database import DatabaseManager
from queries import FootballQueries
from interface import UserInterface

def initialize_database():
    with DatabaseManager() as db:
        # Создать таблицы
        db.create_tables()
        db.add_sample_data()

def main():
    print("СИСТЕМА УПРАВЛЕНИЯ ФУТБОЛЬНЫМИ ДАННЫМИ")
    try:
        # Инициализировать БД
        initialize_database()

        # Создать менеджер БД и объекты
        db_manager = DatabaseManager()
        db_manager.connect()

        queries = FootballQueries(db_manager)
        ui = UserInterface(queries)

        # Запустить интерфейс
        ui.run()

        # Закрыть соединение
        db_manager.disconnect()
        print("Соединение с базой данных закрыто")

    except Exception as e:
        print(f"Критическая ошибка: {e}")
        input("\nНажмите Enter для выхода...")

if __name__ == "__main__":
    main()