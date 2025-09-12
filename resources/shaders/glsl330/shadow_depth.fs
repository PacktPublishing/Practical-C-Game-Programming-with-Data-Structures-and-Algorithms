#version 330

//input from vertex program
in vec2 fragTexCoord;

// Input uniform values from raylib
uniform sampler2D texture0;

//Standard uniforms from Knight
uniform int alphaTest;

void main() {
    // Texel color fetching from texture sampler
    vec4 texelColor = texture(texture0, fragTexCoord);
    if (alphaTest > 0 && texelColor.a < 0.5f) {
        discard; // Discard the fragment if alpha is below threshold
    }

    // Output depth as the fragment's depth
    gl_FragDepth = gl_FragCoord.z;
}
