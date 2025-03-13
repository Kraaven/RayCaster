#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Output fragment color
out vec4 finalColor;

// Input uniform texture containing ray distances
uniform sampler2D hitTexture;

void main()
{
    // Normalize texture coordinates
    vec2 distance_data = vec2(gl_FragCoord.x / 800, 0.0);
    vec4 DistanceValue = texture(hitTexture, distance_data);

    // Extract distance ( distance is stored as sum of all 3 channels)
    float Distance = DistanceValue.x + DistanceValue.y + DistanceValue.z;

    // Compute pixel deviation from screen center
    float screenCenter = 450 / 2.0;
    float PixelDeviation = abs(gl_FragCoord.y - screenCenter);

    // Set final color based on distance and deviation

    if (PixelDeviation * 4 < (225/ Distance)) {
        finalColor = vec4(1.0, 1.0, 1.0, abs((Distance/225)) * 100); // White
    } else {
        finalColor = vec4(0.0, 0.0, 0.0, 1.0); // Black
    }
}