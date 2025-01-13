import asyncio
from django.apps import apps
import json
from channels.generic.websocket import AsyncWebsocketConsumer
from asgiref.sync import sync_to_async
from django.core.serializers.json import DjangoJSONEncoder
from django.utils.timezone import now, timedelta

class DashboardConsumer(AsyncWebsocketConsumer):
    async def connect(self):
        await self.accept()

        # 开启后台任务，每 30 秒发送一次数据
        # EN: Start a background task to send data every 30 seconds
        self.send_task = asyncio.create_task(self.send_device_data_periodically())

    async def disconnect(self, close_code):
        print(f"WebSocket disconnected with code {close_code}")

        # 取消后台任务，防止内存泄漏
        # EN: Cancel the background task to prevent memory leaks
        if hasattr(self, "send_task"):
            self.send_task.cancel()

    @sync_to_async
    def get_devices(self):
        # 从数据库中获取设备数据
        # EN: Get device data from the
        Device = apps.get_model('devices', 'Device')
        return list(Device.objects.all().values('name', 'status', 'last_message', 'updated_at'))

    async def send_device_data_periodically(self):
        """
        每隔 30 秒获取设备数据并发送到客户端。
        ENG: Get device data every 30 seconds and send it to the client.
        """
        try:
            while True:
                devices = await self.get_devices()
                await self.send(text_data=json.dumps(devices, cls=DjangoJSONEncoder))
                await asyncio.sleep(30)  # 等待 30 秒 EN: Wait for 30 seconds
        except asyncio.CancelledError:
            # 捕获任务取消错误，确保任务正确终止 EN: Catch the task cancellation error to ensure the task is terminated correctly
            print("Periodic send task cancelled")

    @sync_to_async
    def get_device_messages(self, device_name=None, limit=None, time_range=None):
        """
        根据客户端筛选条件获取设备历史消息。
        Get device message history based on client filtering conditions.
        :param device_name: 设备名称（可选）Device name (optional)
        :param limit: 获取的消息数量（可选）Number of messages to get (optional)
        :param time_range: 时间范围（tuple: 开始时间，结束时间，格式为datetime，可选） Time range (tuple: start time, end time, format is datetime, optional)
        :return: 筛选后的消息列表 Filtered message list
        """
        DeviceMessage = apps.get_model('devices', 'DeviceMessage')

        # 构建查询
        # Build query
        query = DeviceMessage.objects.all()

        if device_name:
            query = query.filter(device__name=device_name)
        if time_range:
            start_time, end_time = time_range
            query = query.filter(created_at__range=(start_time, end_time))
        if limit:
            query = query.order_by('-created_at')[:limit]

        # 转换为字典列表
        # Convert to a list of dictionaries
        return list(query.values('device__name', 'message', 'created_at'))

    async def receive(self, text_data):
        """
        处理来自客户端的消息，解析筛选条件，并返回筛选后的消息数据。
        Process messages from the client, parse the filter conditions, and return the filtered message data.
        """
        try:
            data = json.loads(text_data)

            # 解析客户端请求
            # Parse client request
            device_name = data.get('device_name')
            limit = data.get('limit')
            time_range = data.get('time_range')  # 传入时间范围，例如 ['2025-01-01T00:00:00', '2025-01-12T00:00:00']

            # 解析时间范围
            # Parse time range
            if time_range:
                start_time = datetime.fromisoformat(time_range[0])
                end_time = datetime.fromisoformat(time_range[1])
                time_range = (start_time, end_time)

            # 获取设备消息
            # Get device messages
            messages = await self.get_device_messages(device_name=device_name, limit=limit, time_range=time_range)

            # 返回筛选后的消息
            # Return filtered messages
            await self.send(text_data=json.dumps({'type': 'device_messages', 'data': messages}, cls=DjangoJSONEncoder))
        except Exception as e:
            # 错误处理
            # Error handling
            await self.send(text_data=json.dumps({'type': 'error', 'message': str(e)}))
