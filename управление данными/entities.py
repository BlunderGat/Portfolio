from dataclasses import dataclass
from datetime import date
from typing import Optional

@dataclass
class Team:
    # Команда
    id: Optional[int] = None
    name: str = ""
    city: str = ""
    coach_name: str = ""
    last_season_position: int = 0

@dataclass
class Player:
    # Игрок
    id: Optional[int] = None
    full_name: str = ""
    current_team_id: Optional[int] = None

@dataclass
class Stadium:
    # Стадион
    id: Optional[int] = None
    name: str = ""
    city: str = ""
    capacity: int = 0

@dataclass
class Match:
    # Матч
    id: Optional[int] = None
    match_date: date = date.today()
    stadium_id: int = 0
    home_team_id: int = 0
    away_team_id: int = 0
    home_score: int = 0
    away_score: int = 0
    status: str = "запланирован"  # запланирован/состоялся/отменен/перенесен

@dataclass
class PlayerPerformance:
    # Выступление игрока в матче
    id: Optional[int] = None
    player_id: int = 0
    match_id: int = 0
    team_id: int = 0
    player_number: Optional[int] = None
    goals_scored: int = 0