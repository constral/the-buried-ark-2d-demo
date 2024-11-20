import xml.etree.ElementTree as ET
import re

# SVG data
svg_data = '''<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<svg
   xmlns="http://www.w3.org/2000/svg">
   <g id="layer1">
       paths go here
   </g>
</svg>'''

# svg content
root = ET.fromstring(svg_data)
# namespace
ns = {'svg': 'http://www.w3.org/2000/svg'}

def process_d_attribute(d_attr):
    # split commands and points to preserve relative and absolute distinctions
    commands = re.findall(r'[MmLlHhVvZz][-0-9., ]+', d_attr)
    
    points = []
    # initialize current position for relative movements
    current_pos = (0, 0)

    for command in commands:
        cmd_type = command[0]
        coords = command[1:].strip()
        coords_list = [tuple(map(float, coord.split(','))) for coord in coords.split()]

        if cmd_type.lower() == 'm':
            # absolute (M) or relative (m)
            if cmd_type == 'M':
                current_pos = coords_list[0]
            else:  # 'm' relative
                current_pos = (current_pos[0] + coords_list[0][0], current_pos[1] + coords_list[0][1])
            points.append(current_pos)
            
            # handle relative lines if more points are present
            for coord in coords_list[1:]:
                current_pos = (current_pos[0] + coord[0], current_pos[1] + coord[1])
                points.append(current_pos)

        elif cmd_type.lower() == 'l':
            # absolute (L) or relative (l)
            for coord in coords_list:
                if cmd_type == 'L':
                    current_pos = coord
                else:  # 'l' relative
                    current_pos = (current_pos[0] + coord[0], current_pos[1] + coord[1])
                points.append(current_pos)

    scaled_points = [(x / 500, y / 500) for x, y in points]
    return scaled_points



def hex_to_rgba(hex_color):
    r = int(hex_color[1:3], 16) / 255.0
    g = int(hex_color[3:5], 16) / 255.0
    b = int(hex_color[5:7], 16) / 255.0
    a = 1.0
    return r, g, b, a



# extract triangles
triangles = []
for path in root.findall('.//svg:path', ns):
    d_attr = path.get('d')
    style_attr = path.get('style')
    
    if d_attr and style_attr:
        points = process_d_attribute(d_attr)

        # only consider triangles
        if len(points) == 3:  
            color_match = re.search(r'fill:(#[0-9a-fA-F]{6})', style_attr)
            color = color_match.group(1) if color_match else '#000000'
            rgba_color = hex_to_rgba(color)

            triangles.append({'vertices': points, 'color': rgba_color})



print("Triangles:")
for tri in triangles:
    v = tri['vertices']
    c = tri['color']
    print(f"Triangle(glm::vec2({v[0][0]:.8f}, {v[0][1]:.8f}), "
          f"glm::vec2({v[1][0]:.8f}, {v[1][1]:.8f}), "
          f"glm::vec2({v[2][0]:.8f}, {v[2][1]:.8f}), "
          f"glm::vec4({c[0]:.2f}, {c[1]:.2f}, {c[2]:.2f}, {c[3]:.2f})),")