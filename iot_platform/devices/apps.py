from django.apps import AppConfig


class DevicesConfig(AppConfig):
    default_auto_field = 'django.db.models.BigAutoField'
    name = 'devices'
    def ready(self):
        from .mqtt_client import client
        client.loop_start()
