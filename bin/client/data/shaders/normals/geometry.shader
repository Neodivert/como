#version 420 core

layout (points) in;
layout (line_strip, max_vertices = 2) out;

in VS_GS_INTERFACE {
	vec4 pos;
	vec3 normal;
} inVertex[];

void main(){
	gl_Position = inVertex[0].pos;
	EmitVertex();

	gl_Position = inVertex[0].pos + vec4( inVertex[0].normal.xyz, 0.0f );
	EmitVertex();
}
