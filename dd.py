import math


def calculate_bearing(lat1, lon1, lat2, lon2):
  phi1 = math.radians(lat1)
  phi2 = math.radians(lat2)
  delta_lambda = math.radians(lon2 - lon1)

  x = math.sin(delta_lambda) * math.cos(phi2)
  y = (math.cos(phi1) * math.sin(phi2)) - (
      math.sin(phi1) * math.cos(phi2) * math.cos(delta_lambda)
  )

  initial_bearing_rad = math.atan2(x, y)
  initial_bearing_deg = math.degrees(initial_bearing_rad)
  compass_bearing = (initial_bearing_deg + 360) % 360
  return compass_bearing


print(calculate_bearing(1,1,10,10))