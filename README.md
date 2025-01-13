# CPC357Project
Green Roof Monitoring System

## Directory Tree
```bash
CPC357Project/
├── .gitignore                # Git ignore file, lists file types to exclude from version control
├── README.md                 # Project overview and description file
├── alarm.cpp                 # C++ source file implementing alarm functionality
├── alarm.h                   # Header file for alarm functionality
├── CPC357Project.ino         # Main Arduino program file
├── iot_platform/             # Django project directory
│   ├── db.sqlite3            # SQLite database file
│   ├── devices/              # Django app directory
│   │   ├── __init__.py       # Initialization file
│   │   ├── admin.py          # Django admin configuration file
│   │   ├── apps.py           # App configuration file
│   │   ├── consumers.py      # WebSocket consumers file
│   │   ├── models.py         # Data models file
│   │   ├── mqtt_client.py    # MQTT client file
│   │   ├── routing.py        # WebSocket routing configuration file
│   │   ├── tests.py          # Test file
│   │   ├── urls.py           # URL routing configuration file
│   │   ├── views.py          # View file
│   ├── iot_platform/         # Django project configuration directory
│   │   ├── __init__.py       # Initialization file
│   │   ├── asgi.py           # ASGI configuration file
│   │   ├── settings.py       # Project settings file
│   │   ├── urls.py           # Project URL routing configuration file
│   │   ├── wsgi.py           # WSGI configuration file
│   ├── manage.py             # Django management command file
│   ├── templates/            # HTML templates directory
│       ├── dashboard.html    # Dashboard page template
├── mynetwork.cpp             # C++ source file implementing network functionality
├── mynetwork.h               # Header file for network functionality
├── RainMonitor.cpp           # C++ source file implementing rain monitoring functionality
├── RainMonitor.h             # Header file for rain monitoring functionality
├── SharedResources.cpp       # C++ source file for shared resources
├── SharedResources.h         # Header file for shared resources
├── SoilMoistureMonitor.cpp   # C++ source file implementing soil moisture monitoring functionality
├── SoilMoistureMonitor.h     # Header file for soil moisture monitoring functionality
```
## Sensors
- Rain Sensor MH-RD
- Soil Moisture Sensor 
- Temperature & Humidity Sensor DH11

## Architecture
- ESP32
- WIFI
- MQTT
- Django
- SQLite
- Django Channels
- WebSocket

## Start the Django Server
```bash
daphne -p 8000 -b 0.0.0.0  iot_platform.asgi:application
```

