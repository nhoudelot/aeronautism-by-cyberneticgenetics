uniform sampler2D forest;
uniform sampler2D shadow;
uniform vec2 shadow_offset;

void main(){
   vec4 forestcolor = texture2D(forest, gl_TexCoord[0].st);

   vec4 shadowcolor = texture2D(shadow, gl_TexCoord[0].st / 20.0 + shadow_offset);

   gl_FragColor = min(forestcolor, (shadowcolor + forestcolor) / 2.0);
}
