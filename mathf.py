import math

def calculate_bearing(lat1, lon1, lat2, lon2):
    """
    Calculates the initial bearing from point 1 to point 2.
    Coordinates must be provided in decimal degrees.
    Returns the bearing in degrees (0° to 360°).
    """
    # 1. Convert decimal degrees to radians
    lat1_rad = math.radians(lat1)
    lat2_rad = math.radians(lat2)
    diff_lon_rad = math.radians(lon2 - lon1)
    
    # 2. Apply the bearing formula components
    x = math.sin(diff_lon_rad) * math.cos(lat2_rad)
    y = math.cos(lat1_rad) * math.sin(lat2_rad) - (
        math.sin(lat1_rad) * math.cos(lat2_rad) * math.cos(diff_lon_rad)
    )
    
    # 3. Calculate the initial bearing in radians and convert to degrees
    initial_bearing_rad = math.atan2(x, y)
    initial_bearing_deg = math.degrees(initial_bearing_rad)
    
    # 4. Normalize the angle to a compass heading (0° to 360°)
    compass_bearing = (initial_bearing_deg + 360) % 360
    
    return compass_bearing

# Example Usage:
# Point 1: Thiruvananthapuram (8.50606, 76.96153)
# Point 2: Kollam (8.88795, 76.59550)
bearing = calculate_bearing(8.50606, 76.96153, 8.88795, 76.59550)
print(f"Bearing: {bearing:.2f}°") 
# Output: Bearing: 316.57°