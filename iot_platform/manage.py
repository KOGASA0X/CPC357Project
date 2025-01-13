#!/usr/bin/env python
"""Django's command-line utility for administrative tasks."""
import os
import sys


def main():
    """Run administrative tasks."""
    os.environ.setdefault('DJANGO_SETTINGS_MODULE', 'iot_platform.settings')
    try:
        from django.core.management import execute_from_command_line
    except ImportError as exc:
        raise ImportError(
            "Couldn't import Django. Are you sure it's installed and "
            "available on your PYTHONPATH environment variable? Did you "
            "forget to activate a virtual environment?"
        ) from exc
    execute_from_command_line(sys.argv)
        # 确保所有应用都已经加载
        # ENG: Ensure all apps are loaded
    from django.apps import apps
    from django.conf import settings
    apps.populate(settings.INSTALLED_APPS)

    # 你可以在这里初始化你的 MQTT 客户端，确保它在 Django 完全加载后运行
    # ENG: You can initialize your MQTT client here to make sure it runs after Django is fully loaded
    try:
        import devices.mqtt_client
    except Exception as e:
        print(f"Error initializing MQTT client: {e}")


if __name__ == '__main__':
    main()
