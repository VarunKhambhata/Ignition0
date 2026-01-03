<pre>
  <b>
 ______                          __                                __     
/\__  _\                     __ /\ \__  __                       /'__`\   
\/_/\ \/       __      ___  /\_\\ \ ,_\/\_\     ___     ___     /\ \/\ \  
   \ \ \     /'_ `\  /' _ `\\/\ \\ \ \/\/\ \   / __`\ /' _ `\   \ \ \ \ \ 
    \_\ \__ /\ \L\ \ /\ \/\ \\ \ \\ \ \_\ \ \ /\ \L\ \/\ \/\ \   \ \ \_\ \
    /\_____\\ \____ \\ \_\ \_\\ \_\\ \__\\ \_\\ \____/\ \_\ \_\   \ \____/
    \/_____/ \/___L\ \\/_/\/_/ \/_/ \/__/ \/_/ \/___/  \/_/\/_/    \/___/ 
               /\____/                                                    
               \_/__/                                                     
  </b>
  <p align="center" >A Graphics Engine</p>
</pre>

Igntion0 engine features:<br>
`Directional Light` `Point Light` `Camera` `Cube` `Sphere` `Plane` `Line` `.Obj Model` `Lit/Unlit Color` `Lit/Unlit Texture` `Input Management` `Engine Linked Shader Framework` `Object Manipulation` `Object Scripting` `Mass Render [instanced rendering]` `Render Qualtiy Controller` `State Based Update & Rendering` `No GPU resource duplication` `Balanced memory,cpu optimization`


<br>

Sample code to create simple rendeirng app using Igntion0 Engine:
```c++
int main() {
	Ignition0::launch("Ignition0", 1600, 900);

    m<Camera> cam = make<Camera>();
    cam->setPosition(0,0,-30);
	cam->setProjection(52, 0.1f, 400.0f);
   
    m<Sphere> sphr = make<Sphere>();
    m<Cube> cube = make<Cube>();

    sphr->setMaterial(make<UnlitColor>(0.0, 0.7, 1.0));
    cube->setMaterial(make<UnlitColor>(1, 0.4, 0.1));
    sphr->setPosition(-3,0,0);
    cube->setPosition(4,2,0);

    m<Scene> scene = make<Scene>();
    scene->add(cam);
    scene->add(sphr);
    scene->add(cube);
	Ignition0::ignite(scene);
	return 0;
}
```



<img width="1781" height="2228" alt="Igntion0SamplesSanpshots" src="https://github.com/user-attachments/assets/1984c792-afc8-419f-a962-185420a432d7" />
