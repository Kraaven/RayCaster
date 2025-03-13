#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Output fragment color
out vec4 finalColor;

// Input uniform texture containing ray distances
uniform sampler2D hitTexture;
uniform float screenWidth;

void main()
{
    float texCoordX = gl_FragCoord.x / screenWidth;
    vec2 distance_data = vec2(gl_FragCoord.x, 0.0);
    vec4 DistanceValue = texture(hitTexture, distance_data/800);
    float Distance  = DistanceValue.x + DistanceValue.y + DistanceValue.z;
    Distance = Distance/2;

    float PixelDeviation = 0;
    if(gl_FragCoord.y > 225){
        //PixelDeviation -= gl_FragCoord.y;
        finalColor = vec4(1.0,1.0,1.0,1.0);
    }
    else{
        //PixelDeviation = 255 - gl_FragCoord.y;
        finalColor = vec4(0.0,0.0,0.0,0.0);
    }

    //finalColor = vec4(PixelDeviation/1000,PixelDeviation/1000,PixelDeviation/1000,1.0);
    //225
    // if 0, then 255 is lit
    // if 255 then 0 is lit

}
