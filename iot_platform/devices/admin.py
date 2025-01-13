from django.contrib import admin
from .models import Device, DeviceMessage

@admin.register(Device)
class DeviceAdmin(admin.ModelAdmin):
    list_display = ('name', 'status', 'last_message', 'updated_at')
    list_filter = ('status',)  # 按状态过滤 ENG: Filter by status
    search_fields = ('name',)  # 按名称搜索 ENG: Search by name

@admin.register(DeviceMessage)
class DeviceMessageAdmin(admin.ModelAdmin):
    list_display = ('device', 'message', 'created_at')  # 显示的字段 ENG: Displayed fields
    list_filter = ('device',)  # 过滤选项 ENG: Filter options
    search_fields = ('device__name', 'message')  # 支持通过设备名称和消息内容搜索 ENG: Support search by device name and message content
    ordering = ('-created_at',)  # 默认排序，按创建时间倒序 ENG: Default sorting, in descending order of creation time
