#version 460 core
struct Vertex {
  float pos[3];
  float col[3];
  float norm[3];
  float vcoords[2];
};

layout (std430, binding = 1) readonly buffer vertices{
  Vertex inVerts[];
};

layout (std140, binding = 0) uniform MatrixBlock{
  mat4 projection;
  mat4 view;
  mat4 model;
} Matrices;

vec3 getPosition(int i) {
  return vec3(inVerts[i].pos[0], inVerts[i].pos[1], inVerts[i].pos[2]);
}

vec3 getColor(int i) {
  return vec3(inVerts[i].col[0], inVerts[i].col[1], inVerts[i].col[2]);
}

vec3 getNormals(int i) {
  return vec3(inVerts[i].norm[0], inVerts[i].norm[1], inVerts[i].norm[2]);
}

vec2 getVCoords(int i) {
  return vec2(inVerts[i].vcoords[0], inVerts[i].vcoords[1]);
}

out vec3 ourColor;
out vec3 norm;
out vec2 ftex_coords;

void main()
{
  vec3 pos = getPosition(gl_VertexID);
  gl_Position = Matrices.projection * Matrices.view * Matrices.model * vec4(pos, 1.0);
  //gl_Position = vec4(pos, 1.0);
  ftex_coords = getVCoords(gl_VertexID);
  norm = getNormals(gl_VertexID);

  ourColor = getColor(gl_VertexID);
}
