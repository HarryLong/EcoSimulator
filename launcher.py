from subprocess import call
from multiprocessing import Process

def start_instance():
  call(["/home/harry/workspaces/qt-workspace/EcoSimulator-build/EcoSim",""])

for i in range(0,15):
  Process(target=start_instance).start()
