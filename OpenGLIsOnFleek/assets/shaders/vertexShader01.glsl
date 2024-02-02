// Vertex shader
#version 420

//uniform mat4 MVP;
uniform mat4 matView;
uniform mat4 matProjection;
uniform mat4 matModel;			// Later...
uniform mat4 matModel_IT;		// Inverse transpose of the model matrix

//uniform vec3 modelScale;
//uniform vec3 modelOffset;

in vec4 vPos;		
in vec4 vCol;
in vec4 vNormal;	
in vec2 vTextureCoords;		// NOTE this is a vec2 not vec4

out vec4 colour;
out vec4 vertexWorldPos;	
out vec4 vertexWorldNormal;
out vec2 textureCoords;		// ADDED

uniform bool bUseHeightMap;
uniform sampler2D heightMapSampler;		// Texture unit 20
uniform float heightScale;
uniform vec2 UVOffset;

void main()
{
	vec4 vertexModelPosition = vPos;	

	vec2 UVFinal = vTextureCoords.st;
	
	if ( bUseHeightMap )
	{
		// Greyscale (black and white) heightmap image
		// NvF5e_height_map.bmp
//		vec3 heightRGB = texture( heightMapSampler, vTextureCoords.st ).rgb;
//		float height = (heightRGB.r + heightRGB.g + heightRGB.b) / 3.0f;
		UVFinal += UVOffset.yx;
		float height = texture( heightMapSampler, UVFinal.st ).r;
		
		// Adjust the height of the y axis from this "colour"
//		const float SCALEADJUST = 25.0f;
//		vertexModelPosition.y += ( height * SCALEADJUST );
		vertexModelPosition.y += ( height * heightScale );
	}
	
	
//	gl_Position = MVP * vec4(finalPos, 1.0);
//	gl_Position = MVP * vertModelPosition;
	mat4 matMVP = matProjection * matView * matModel;
	gl_Position = matMVP * vec4(vertexModelPosition.xyz, 1.0);
	
		
	// Rotate the normal by the inverse transpose of the model matrix
	// (so that it only is impacted by the rotation, not translation or scale)
	vertexWorldNormal = matModel_IT * vec4(vNormal.xyz, 1.0f);
	vertexWorldNormal.xyz = normalize(vertexWorldNormal.xyz);
	vertexWorldNormal.w = 1.0f;
	
	vertexWorldPos = matModel * vec4( vertexModelPosition.xyz, 1.0f);
	
	colour = vCol;
	
	// Copy the UV coordinates unchanged (to the fragment shader)
//	textureCoords = vTextureCoords;
	textureCoords = UVFinal;
}
