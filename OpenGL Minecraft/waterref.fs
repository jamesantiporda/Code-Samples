/******************************************************************************
 * This fragment shader improves on the previous one by implementing normal
 * mapping -- the normal vector is looked up from a second texture map (the
 * "normal map"), then this normal is transformed to the original model's
 * coordinate space using a TBN matrix (computed in the vertex shader).
 *
 * Happy hacking! - eric
 *****************************************************************************/

#version 330 core

in vec3 shaderPosition;
in mat3 shaderTBN;
in vec2 shaderTexCoord;
in vec3 shaderLightPosition;
in vec3 shaderSpotlightDirection;
uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D specularMap;
// uniform float innerAngle;
// uniform float outerAngle;
uniform float uniformAmbientIntensity;
uniform float specularShininess;
uniform float currentLinearCoeff;
uniform float currentQuadraticCoeff;
out vec4 fragmentColor;

///////////////////////////////////////////////////////////////////////////////
// added for shadow mapping
in vec4 shaderLightSpacePosition;
uniform sampler2D shadowMap;
uniform int shadowOnUniform;
uniform int sampleSide;
uniform float sampleAreaSize;

///////////////////////////////
// added for reflections
in vec4 clipSpace;

// float inShadow()
// {
//     // perform perspective division and rescale to the [0, 1] range to get the coordinates into the depth texture
//     vec3 position = shaderLightSpacePosition.xyz / shaderLightSpacePosition.w;
//     position = position * 0.5f + 0.5f;

//     // if the position is outside the light-space frustum, do NOT put the
//     // fragment in shadow, to prevent the scene from becoming dark "by default"
//     // (note that if you have a spot light, you might want to do the opposite --
//     // that is, everything outside the spot light's cone SHOULD be dark by default)
//     if (position.x < 0.0f || position.x > 1.0f
//         || position.y < 0.0f || position.y > 1.0f
//         || position.z < 0.0f || position.z > 1.0f)
//     {
//         return 1.0f;
//     }

//     float pcfDepth;

//     float shadow = 0.0f;
//     vec2 texelSize = sampleAreaSize / textureSize(shadowMap, 0);

//     // add a bias to prevent shadow acne
//     float bias = 0.000005f;

//     for(int i = -(sampleSide/2); i <= sampleSide/2; i++)
//     {
//         for(int j = -(sampleSide/2); j <= sampleSide/2; j++)
//         {
//             pcfDepth = texture(shadowMap, position.xy + vec2(i, j) * texelSize).r;
//             if(pcfDepth + bias > position.z)
//             {
//                 shadow += 1.0f;
//             }
//         }
//     }

//     // access the shadow map at this position
//     float shadowMapZ = texture(shadowMap, position.xy).r;

//     //shadowMapZ += bias;

//     // if (shadow == 0.0f)
//     // {
//     //    return 1.0f;
//     // }

//     // if the depth stored in the texture is less than the current fragment's depth, we are in shadow
//     return shadow/(sampleSide * sampleSide);
// }
// ///////////////////////////////////////////////////////////////////////////////

void main()
{
    // define some constant properties for the light
    // (you should really be passing these parameters into the shader as uniform vars instead)
    vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);  // diffuse
    float ambientIntensity = uniformAmbientIntensity;            // ambient
    float specularIntensity = texture(specularMap, shaderTexCoord).x;   // specular (better implementation: look this up from a specular map!)
    float specularPower = specularShininess;               // specular exponent

    // look up the normal from the normal map, then reorient it with the current model transform via the TBN matrix
    vec3 textureNormal = vec3(texture(normalMap, shaderTexCoord));
    textureNormal = normalize(textureNormal * 2.0f - 1.0f);  // convert range from [0, 1] to [-1, 1]
    vec3 normalDir = normalize(shaderTBN * textureNormal);

    // calculate ambient
    vec3 lightAmbient = lightColor * ambientIntensity;

    // calculate diffuse
    vec3 lightDir = normalize(shaderLightPosition - shaderPosition);
    vec3 lightDiffuse = max(dot(normalDir, lightDir), 0.0f) * lightColor;

    // calculate specular
    vec3 viewDir = normalize(-shaderPosition);
    vec3 reflectDir = reflect(-lightDir, normalDir);
    vec3 lightSpecular = pow(max(dot(reflectDir, viewDir), 0), specularPower) * lightColor * specularIntensity;

    // calculate if fragment is in shadow
    // float cosTheta = dot(lightDir, normalize(-shaderSpotlightDirection));
    // float numerator = cosTheta - cos(radians(outerAngle));
    // float denomenator = cos(radians(innerAngle)) - cos(radians(outerAngle));
    // float spotlightI = clamp(numerator/denomenator, 0.0f, 1.0f);

    // float spotlightI = 1.0f;

    // if(cosTheta <= cos(radians(innerAngle)))
    // {
    //     spotlightI = 0.0f;
    // }

    float lightDistance = length(shaderLightPosition - shaderPosition);
    float attenuation = (1.0f/(1.0f + currentLinearCoeff * lightDistance + currentQuadraticCoeff * lightDistance * lightDistance));

    // if(shadowOnUniform == 1)
    // {
    // ///////////////////////////////////////////////////////////////////////////
    // // zero-out the diffuse and specular components if the fragment is in shadow
    //     lightDiffuse = lightDiffuse * inShadow();
    //     lightSpecular = lightSpecular * inShadow();
    // ///////////////////////////////////////////////////////////////////////////
    // }

    vec2 ndc = (clipSpace.xy/clipSpace.w)/2.0 + 0.5;
    vec2 reflectTexCoords = vec2(ndc.x, -ndc.y);

    // compute final fragment color
    fragmentColor = vec4((lightAmbient + lightDiffuse) * attenuation, 1.0f) * texture(diffuseMap, shaderTexCoord); //* vec4(0.0, 0.0, 1.0, 0.0);
    // fragmentColor = vec4(1.0f, 1.0f, 1.0f, 1.0f) * texture(diffuseMap, shaderTexCoord); //* vec4(0.0, 0.0, 1.0, 0.0);
}
