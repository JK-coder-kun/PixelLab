#version 330 core

in vec2 TexCoord;
uniform sampler2D texture1; // Reference to the 2D image texture
uniform float greyIntensity; // Threshold for edge detection
//uniform vec3 edgeColor; // Color for edges
uniform float pixelBlocks ;


void main() {
    float quantizationLevels =pixelBlocks*16;
    float edgeThreshold = greyIntensity*1;
    vec3 edgeColor = vec3(1);
  // Sample the color from the texture at the current pixel
  vec4 color = texture2D(texture1, gl_PointCoord);

  // Apply Sobel filter for edge detection (adjust kernel size and values if needed)
  vec2 sobelX = vec2(-1, 1);
  vec2 sobelY = vec2(1, -1);

  float gx = dot(texture2D(texture1, gl_PointCoord + sobelX).rgb, vec3(0.299, 0.587, 0.114));
  gx += dot(texture2D(texture1, gl_PointCoord - sobelX).rgb, vec3(0.299, 0.587, 0.114));

  float gy = dot(texture2D(texture1, gl_PointCoord + sobelY).rgb, vec3(0.299, 0.587, 0.114));
  gy += dot(texture2D(texture1, gl_PointCoord - sobelY).rgb, vec3(0.299, 0.587, 0.114));

  // Combine gradients for magnitude
  float gradient = sqrt(gx * gx + gy * gy);

  // Edge detection with threshold
  float isEdge = smoothstep(0.0, edgeThreshold, gradient); // Smooth transition at the edge
  vec3 edgeFactor = vec3(isEdge); // Apply edge color based on transition

  // Quantization based on brightness
  float brightness = dot(color.rgb, vec3(0.299, 0.587, 0.114));
  float quantizedBrightness = floor(brightness * quantizationLevels) / (quantizationLevels - 1.0);

  // Apply edge color, quantization, and preserve alpha
  vec3 finalColor = mix(color.rgb, edgeColor, edgeFactor)*vec3(quantizedBrightness);
  gl_FragColor = vec4(finalColor, color.a);
}


// (Optional) Define a uniform value for color quantization
//uniform float uQuantize = 8.0;
