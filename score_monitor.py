import urllib2
import urllib
import RPi.GPIO as GPIO
GPIO.setmode(GPIO.BCM)

team_1_sensor = 18
team_1_sensor_last_state = 0
team_1_sensor_state = 0
team_1_sensor_count = 0
team_1_sensor_event = 0

team_2_sensor = 23
team_2_sensor_last_state = 0
team_2_sensor_state = 0
team_2_sensor_count = 0
team_2_sensor_event = 0

team_reset_sensor = 24
team_reset_sensor_last_state = 0
team_reset_sensor_state = 0
team_reset_sensor_count = 0
team_reset_sensor_event = 0

GPIO.setup(team_1_sensor, GPIO.IN, pull_up_down = GPIO.PUD_UP)
GPIO.setup(team_2_sensor, GPIO.IN, pull_up_down = GPIO.PUD_UP)
GPIO.setup(team_reset_sensor, GPIO.IN, pull_up_down = GPIO.PUD_UP)

while True:
  team_1_sensor_state = GPIO.input(team_1_sensor)
  team_2_sensor_state = GPIO.input(team_2_sensor)
  team_reset_sensor_state = GPIO.input(team_reset_sensor)

  if (team_1_sensor_state != team_1_sensor_last_state):
    team_1_sensor_last_state = team_1_sensor_state
    team_1_sensor_count = 0
    team_1_sensor_event = 0
  else:
    team_1_sensor_count = team_1_sensor_count + 1
    if (team_1_sensor_count > 100 and team_1_sensor_state == 1 and team_1_sensor_event == 0):
      team_1_sensor_event = 1
      query_args = { 'Team':'1' }
      url = 'http://192.168.1.7:3000/score'
      data = urllib.urlencode(query_args)
      request = urllib2.Request(url, data)
      response = urllib2.urlopen(request).read()
      print response

  if (team_2_sensor_state != team_2_sensor_last_state):
    team_2_sensor_last_state = team_2_sensor_state
    team_2_sensor_count = 0
    team_2_sensor_event = 0
  else:
    team_2_sensor_count = team_2_sensor_count + 1
    if (team_2_sensor_count > 100 and team_2_sensor_state == 1 and team_2_sensor_event == 0):
      team_2_sensor_event = 1
      query_args = { 'Team':'2' }
      url = 'http://192.168.1.7:3000/score'
      data = urllib.urlencode(query_args)
      request = urllib2.Request(url, data)
      response = urllib2.urlopen(request).read()
      print response
     
  if (team_reset_sensor_state != team_reset_sensor_last_state):
    team_reset_sensor_last_state = team_reset_sensor_state
    team_reset_sensor_count = 0
    team_reset_sensor_event = 0
  else:
    team_reset_sensor_count = team_reset_sensor_count + 1
    if (team_reset_sensor_count > 100 and team_reset_sensor_state == 1 and team_reset_sensor_event == 0):
      team_reset_sensor_event = 1
      query_args = {}
      url = 'http://192.168.1.7:3000/score/reset'
      data = urllib.urlencode(query_args)
      request = urllib2.Request(url, data)
      response = urllib2.urlopen(request).read()
      print response

GPIO.cleanup()
