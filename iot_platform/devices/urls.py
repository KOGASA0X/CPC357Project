from django.urls import path
from . import views  # Import views from the app

urlpatterns = [
    path('', views.dashboard, name='dashboard'),  # Route for the dashboard
]
