# Normal Map OPGL

This project demonstrates the use of normal maps to improve the visual quality of 3D objects in computer graphics. It provides a program that allows users to see the difference in quality by applying normal
maps. There are shown 2 practices to generate a normal map:
One method uses a diffuse map, while the other method uses a height map. The project also provides a way for users to interact with a 3D cube and observe the difference between the base texture created and
the texture with the normal map.

## Getting Started

The program will open a window displaying a 3D cube with a texture applied to it. It uses a normal map generated from a diffuse map (diffuse.png) in the `textures` folder by default.
The cube is interactive, and you can manipulate it using various keyboard inputs.

### Keyboard Controls

- Press **N**: Toggle between the base texture and the normal map texture.
- Use other keys as specified in the program to manipulate the cube's position, rotation, and other parameters.

## Generating Normal Maps

This project provides two methods for generating normal maps:

1. **Diffuse Map Method**: The program can use a normal map created from the provided diffuse map (diffuse.png), but this method is substantially wrong for generating normal maps.

2. **Height Map Method**: The preferred method is to use a height map(h_map_noise.jpg) to generate the normal map.
## Viewing the Generated Normal Maps

The original normal maps can be found in the `textures` folder:

- `normal.png`: This is the normal map generated using the diffuse map method.
- `h_map_noise_normail.jpg`: This is another normal map generated using GIMP.

You can compare the generated normal_map.png with the original normal maps.
