#version 330 core

uniform sampler2D texture1;
uniform sampler2D texture2;

in vec2 TexCoord;

out vec4 fragColor;

void main()
{
	//float alpha = mix(texture1.a, texture2.a, mixture);
	//fragColor = mix(texture(texture1, TexCoord), texture(texture2, vec2(TexCoord.x, 1 - TexCoord.y)), 0.2);
	fragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
}