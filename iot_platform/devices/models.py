from django.db import models

class Device(models.Model):
    name = models.CharField(max_length=255)
    status = models.BooleanField(default=False)
    last_message = models.TextField(blank=True, null=True)
    updated_at = models.DateTimeField(auto_now=True)

    def __str__(self):
        return self.name

class DeviceMessage(models.Model):
    device = models.ForeignKey(Device, on_delete=models.CASCADE, related_name='messages')  # 关联设备 Associated device
    message = models.TextField()  # 消息内容 Message content
    created_at = models.DateTimeField(auto_now_add=True)  # 消息时间戳 Message timestamp

    def __str__(self):
        return f"Message from {self.device.name} at {self.created_at}"
