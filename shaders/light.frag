#version 330

in vec2 fragTexCoord;
in vec4 fragColor;
out vec4 finalColor;

uniform sampler2D texture0;
uniform vec4 colDiffuse;

uniform vec2 light_pos;
uniform float radius;

void main() {

  vec4 texelColor = texture(texture0, fragTexCoord);
  float dist = distance(gl_FragCoord.xy, light_pos);
  float brightness = clamp(1.0 - dist / radius, 0.0, 1.0);

  finalColor = texelColor * colDiffuse * fragColor * brightness;

}