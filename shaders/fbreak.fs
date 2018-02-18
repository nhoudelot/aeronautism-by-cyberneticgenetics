uniform sampler2D texture;
uniform float radius1;
uniform float radius2;

void main(){
   const vec2 burncenter1 = vec2(0.32, 0.64);
   const vec2 burncenter2 = vec2(0.80, 0.32);
   const vec4 red         = vec4(1.0, 0.0, 0.0, 1.0);
   const vec4 white       = vec4(1.0);

   float dist1  = distance(burncenter1, gl_TexCoord[0].st);
   float dist2  = distance(burncenter2, gl_TexCoord[0].st);

   float f1a    = smoothstep(radius1 + 0.05, radius1 - 0.05, dist1);
   float f1b    = smoothstep(radius1 - 0.05, radius1 - 0.08, dist1);
   float f2a    = smoothstep(radius2 + 0.05, radius2 - 0.05, dist2);
   float f2b    = smoothstep(radius2 - 0.05, radius2 - 0.08, dist2);

   vec4 texel   = texture2D(texture, gl_TexCoord[0].st);
   vec4 mixed1a = mix(texel, red, f1a);
   vec4 mixed1b = mix(red, white, f1b);
   vec4 mixed2a = mix(texel, red, f2a);
   vec4 mixed2b = mix(red, white, f2b);

   vec4 mixed1 = max(mixed1a, mixed1b);
   if(mixed1 == red) mixed1 = mixed1a;

   vec4 mixed2 = max(mixed2a, mixed2b);
   if(mixed2 == red) mixed2 = mixed2a;

   gl_FragColor = max(mixed1, mixed2);
}
