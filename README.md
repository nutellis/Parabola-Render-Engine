# Parabola Render Engine

This is a project I have been working on and off since 2015-2016 and it was my first attempt on a render engine. It has influences from Unreal Engine and is my little Frankenstein's monster where I experimented with everything I could grasp.
It probably won't build on any other machine and, I am not going to lie, I am not even sure why it still builds on mine :)

The project was moved on GitHub from Bitbucket in 2024. There are monstrocities in there that I tried to contain when made the repo public. I hope that noone will look for them.

# Features

* Cook-Torrance microfacet BRDF
* Cascade Shadow Maps w/ PCSS and custom PCF
* Ambient Occlusion based on NVIDIA's HBAO+ implementation
* Per Material Batch rendering (MultiDrawArraysIndirect)
* Frustum Fitting & Frustrum Culling
* Simple SceneGraph w/ mesh list per object
* Axis-Aligned Bounding Box 
* Multiple Cameras
* and probably more that I totally forget

## Other features
* Custom templated dynamic array implementation
* Custom Math library (Vector & Matrix Math w/ intrinsic)

* Custom Logger
* Custom FBX importer (RIP I broke it beyond repair)
* Custom .obj importer (replaced with tinyobj because it was very slow)
* Attempt for a configuration setup using .ini files (never finished it)
* Serialization and archival of arbitrary data (deprecated)
* Attempt for Reflection based on Unreal's implementation using generated includes
* and many more half written features


## Dependencies
* glfw
* Dear ImGui
* tinyobj

# Screenshots
Shadows + AO
<img width="2050" height="1191" alt="shadow + ao on" src="https://github.com/user-attachments/assets/a8ea368c-4fdc-4ec1-a354-97e453b79f75" />
<img width="2050" height="1191" alt="sponza view" src="https://github.com/user-attachments/assets/2ca26d3a-543f-4ace-b724-37bcae689aa1" />


AO only / BRDF only
|<img width="2050" height="1191" alt="ao on" src="https://github.com/user-attachments/assets/b526e73e-e0ba-4112-80f0-fc97d739ba98" />|<img width="2050" height="1191" alt="brdf only" src="https://github.com/user-attachments/assets/03e08caa-c340-4f74-89e9-944084141cec" />|
|--|--|

## Cascade Shadow Maps debug view
|<img width="2050" height="1191" alt="cascade aabb debug" src="https://github.com/user-attachments/assets/471cb3e4-a987-448b-8121-ff2f510fb237" />|<img width="2050" height="1191" alt="cascade solo with ligth view" src="https://github.com/user-attachments/assets/fb7a512d-bad7-481b-bd19-35a1bc839ecb" /> |<img width="2050" height="1191" alt="cascade frustrum debug" src="https://github.com/user-attachments/assets/129d5265-269d-4722-9691-9662bd47767b" />|
|--|--|--|

## AO debug view
<img width="2050" height="1191" alt="hbao debug" src="https://github.com/user-attachments/assets/a54ae0df-09de-46eb-9f23-58abd3e370fd" />

## Frustrum Culling
<img width="2050" height="1192" alt="debug camera frustrum culling" src="https://github.com/user-attachments/assets/00bebb6b-8172-428f-90a4-21cbaaf72f8e" />


P.S.
Most of the features are somewhat buggy:
* PCSS barerly works (some angles refuse to behave)
* AO is not very stable and I know for a fact that I have messed up on the ray casting part
* Cascades work pretty well but refuse to cooperate without a "big enough" distance on the light, although it shouldn't matter (directional light)
* Debug view crashes if both edges and planes are selected but the order matters :)
* .obj loading works fine but the transfer to my asset structure makes some textures to become lost (it is visible on the plants in sponza)
* The logger although works on the console, it refuses to work with ImGui without any explanation. It just stopped working.
* Cast shadow and light size inputs do nothing ^_^
