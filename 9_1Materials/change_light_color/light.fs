#version 330 core
out vec4 FragColor;

uniform vec3 light_color;

void main()
{
    FragColor = vec4(light_color, 1.0f);
}

// 希望灯一直保持明亮 不受修改物体的顶点或者片段着色器后，使灯的位置或者颜色发生改变的影响
// 因此需要另外创建一套顶点着色器和片段着色器
// 顶点着色器与物体的顶点着色器相同
// 片段着色器给灯定义了一个不变的常量白色 保证灯的颜色一直是亮的
// 我的理解:修改源代码中的光源颜色 不会改变这个所谓“光源”物体的颜色，他只是被具象为一个光源物体
// 实际影响物体颜色的是源代码中的物体颜色与光源颜色的设置
