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
    // Use proper texture coordinates
    vec2 distance_data = vec2(gl_FragCoord.x / 800.0, 0.0);
    
    // Get the distance value from the texture (only red channel)
    vec4 distanceValue = texture(hitTexture, distance_data);
    float distance = distanceValue.r * 255.0; // Unnormalize the distance
    
    // Compute pixel deviation from screen center
    float screenCenter = 450.0 / 2.0;
    float pixelDeviation = abs(gl_FragCoord.y - screenCenter);
    
    // Calculate wall height/thickness inversely proportional to distance
    float wallThickness = 225.0 / max(distance, 0.1); // Avoid division by zero
    
    // Set final color based on distance and deviation
    if (pixelDeviation / 40 < wallThickness) {
        // Calculate brightness based on distance
        float brightness = clamp(1.0 - (distanceValue.r), 0.01, 1.0);
        finalColor = vec4(brightness, brightness, brightness, 1.0);
    } else {
        finalColor = vec4(0.0, 0.0, 0.0, 1.0); // Black
    }
}