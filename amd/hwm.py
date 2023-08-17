# Importing Libraries
from serial import Serial
import time
import psutil
import struct
import json
import pyamdgpuinfo

maxGpuMem=12000
arduinoPort="/dev/ttyUSB0"

arduino = Serial(port=arduinoPort, baudrate=9600, timeout=1)
n_devices = pyamdgpuinfo.detect_gpus()
gpu = pyamdgpuinfo.get_gpu(0)
while True:
    gpuTemp = int(gpu.query_temperature())
    gpuUsage = int(gpu.query_load()*100)
    gpuMem = int(gpu.query_vram_usage())
    gpuMemPercentUsage = int((gpuMem/1000000)/maxGpuMem*100)

    cpuJson = json.dumps(psutil.sensors_temperatures())
    cpuTemp = int(json.loads(cpuJson)["k10temp"][0][1])

    ram = int(psutil.virtual_memory().percent)

    cpu = int(psutil.cpu_percent())
    
    swap = int(psutil.swap_memory().percent)
    
    obj_Disk = psutil.disk_usage('/')
    diskusage = int(obj_Disk.percent)

    print(f'cpu: {cpu}%, gpu: {gpuUsage}%, gpuMemPercentUsage: {gpuMemPercentUsage}%, ram: {ram}%, cpuTemp: {cpuTemp}, gpuTemp: {gpuTemp}, swap: {swap}%, diskUsage: {diskusage}%')

    arduino.write(struct.pack('BBBBBBBB',cpu,gpuUsage,gpuMemPercentUsage,ram,swap,diskusage,cpuTemp,gpuTemp))
    time.sleep(1)
    
gpu.stop_utilisation_polling()
