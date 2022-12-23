# Importing Libraries
from serial import Serial
import time
from nvitop import Device
import struct
import psutil
import json

maxGpuMem=12000
arduinoPort="/dev/ttyUSB0"

arduino = Serial(port='/dev/ttyUSB1', baudrate=9600, timeout=.1)
nvidia0 = Device(0)
while True:
    gpu = nvidia0.as_snapshot()
    gpuTemp = gpu.temperature
    gpuUsage = gpu.gpu_utilization
    gpuMem = int(gpu.memory_utilization)

    cpuJson = json.dumps(psutil.sensors_temperatures())
    cpuTemp = int(json.loads(cpuJson)["k10temp"][0][1])

    ram = int(psutil.virtual_memory().percent)

    cpu = int(psutil.cpu_percent())

    swap = int(psutil.swap_memory().percent)

    print(f'cpu: {cpu}%, gpu: {gpuUsage}%, gpuMemPercentUsage: {gpuMemPercentUsage}%, ram: {ram}%, cpuTemp: {cpuTemp}, gpuTemp: {gpuTemp}, swap: {swap}%')

    arduino.write(struct.pack('BBBBBBB',cpu,gpuUsage,gpuMemPercentUsage,ram,swap,cpuTemp,gpuTemp))
    time.sleep(1)
    
