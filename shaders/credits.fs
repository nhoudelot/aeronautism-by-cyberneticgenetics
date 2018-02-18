uniform sampler2D who;
uniform float time;
uniform float aspect;

void main(){

  /* Set the texture position in the screen */
  float height = 1.0 / aspect;
  vec2 pos = vec2(gl_TexCoord[0].s, gl_TexCoord[0].t / height - (1.0 - height / 2.0));
  pos = clamp(pos, 0.0, 1.0);
  vec4 color = texture2D(who, pos);

  /* Some color effect */
  float green = sin(gl_TexCoord[0].s * time) / 2.0 + 0.5 + cos(gl_TexCoord[0].t * time) / 2.0;
  float red = 1/gl_TexCoord[0].s * sqrt(gl_TexCoord[0].t / 2.0);
  vec4 c2 = vec4(red, green, 0.0, 1.0);

  gl_FragColor = c2 * color;
}
