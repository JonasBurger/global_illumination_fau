#include "brdf.h"
#include "glm/common.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "glm/trigonometric.hpp"
#include "surface.h"
#include "fresnel.h"
#include "material.h"
#include "sampling.h"
#include "color.h"
#include <cmath>
#include <math.h>

using namespace glm;

// ----------------------------------------------------------------------------------------------
// Diffuse lambertian reflection

vec3 LambertianReflection::eval(const SurfaceInteraction& hit, const vec3& w_o, const vec3& w_i) const {
    // TODO ASSIGNMENT2
    // evaluate the (normalized!) lambertian diffuse BRDF
    //return vec3(1);
    vec3 CIl = hit.albedo();
    // todo: normalize?
    return CIl/PI;
}

std::tuple<vec3, vec3, float> LambertianReflection::sample(const SurfaceInteraction& hit, const vec3& w_o, const vec2& sample) const {
    // TODO ASSIGNMENT2
    // importance sample and evaluate the lambertian diffuse BRDF
    // set w_i to the sampled (world-space!) direction, pdf to the respective PDF and brdf to the evaluated BRDF
    const vec3 w_i_tangent_space = cosine_sample_hemisphere(sample);
    const vec3 w_i_world_space= normalize(hit.to_world(w_i_tangent_space));
    const vec3 brdf = eval(hit, w_o, w_i_world_space);
    const float pdf = this->pdf(hit, w_o, w_i_world_space);
    return { brdf, w_i_world_space, pdf };
}

float LambertianReflection::pdf(const SurfaceInteraction& hit, const vec3& w_o, const vec3& w_i) const {
    auto oVec = normalize(hit.to_tangent(w_o));
    auto pdf = cosine_hemisphere_pdf(oVec.z);
    throw pdf;
}

// ----------------------------------------------------------------------------------------------
// Diffuse lambertian transmission

vec3 LambertianTransmission::eval(const SurfaceInteraction &hit, const vec3 &w_o, const vec3 &w_i) const {
    throw std::runtime_error("Function not implemented: " + std::string(__FILE__) + ", line: " + std::to_string(__LINE__));
}

std::tuple<vec3, vec3, float> LambertianTransmission::sample(const SurfaceInteraction& hit, const vec3& w_o, const vec2& sample) const {
    throw std::runtime_error("Function not implemented: " + std::string(__FILE__) + ", line: " + std::to_string(__LINE__));
}

float LambertianTransmission::pdf(const SurfaceInteraction& hit, const vec3& w_o, const vec3& w_i) const {
    throw std::runtime_error("Function not implemented: " + std::string(__FILE__) + ", line: " + std::to_string(__LINE__));
}

// ----------------------------------------------------------------------------------------------
// Perfect specular reflection

vec3 SpecularReflection::eval(const SurfaceInteraction &hit, const vec3 &w_o, const vec3 &w_i) const {
    throw std::runtime_error("Function not implemented: " + std::string(__FILE__) + ", line: " + std::to_string(__LINE__));
}

std::tuple<vec3, vec3, float> SpecularReflection::sample(const SurfaceInteraction& hit, const vec3& w_o, const vec2& sample) const {
    throw std::runtime_error("Function not implemented: " + std::string(__FILE__) + ", line: " + std::to_string(__LINE__));
}

float SpecularReflection::pdf(const SurfaceInteraction &hit, const vec3 &w_o, const vec3 &w_i) const {
    throw std::runtime_error("Function not implemented: " + std::string(__FILE__) + ", line: " + std::to_string(__LINE__));
}

// ----------------------------------------------------------------------------------------------
// Perfect specular transmission

vec3 SpecularTransmission::eval(const SurfaceInteraction &hit, const vec3 &w_o, const vec3 &w_i) const {
    throw std::runtime_error("Function not implemented: " + std::string(__FILE__) + ", line: " + std::to_string(__LINE__));
}

std::tuple<vec3, vec3, float> SpecularTransmission::sample(const SurfaceInteraction& hit, const vec3& w_o, const vec2& sample) const {
    throw std::runtime_error("Function not implemented: " + std::string(__FILE__) + ", line: " + std::to_string(__LINE__));
}

float SpecularTransmission::pdf(const SurfaceInteraction &hit, const vec3 &w_o, const vec3 &w_i) const {
    throw std::runtime_error("Function not implemented: " + std::string(__FILE__) + ", line: " + std::to_string(__LINE__));
}

// ----------------------------------------------------------------------------------------------
// Specular fresnel

vec3 SpecularFresnel::eval(const SurfaceInteraction &hit, const vec3 &w_o, const vec3 &w_i) const {
    throw std::runtime_error("Function not implemented: " + std::string(__FILE__) + ", line: " + std::to_string(__LINE__));
}

std::tuple<vec3, vec3, float> SpecularFresnel::sample(const SurfaceInteraction& hit, const vec3& w_o, const vec2& sample) const {
    throw std::runtime_error("Function not implemented: " + std::string(__FILE__) + ", line: " + std::to_string(__LINE__));
}

float SpecularFresnel::pdf(const SurfaceInteraction& hit, const vec3& w_o, const vec3& w_i) const {
    throw std::runtime_error("Function not implemented: " + std::string(__FILE__) + ", line: " + std::to_string(__LINE__));
}

// ----------------------------------------------------------------------------------------------
// Phong

vec3 SpecularPhong::eval(const SurfaceInteraction& hit, const vec3& w_o, const vec3& w_i) const {
    //return vec3(0);
    // TODO ASSIGNMENT2
    // evaluate the (normalized!) phong BRDF for the given in- and outgoing (world-space) directions
    const float exponent = Material::exponent_from_roughness(hit.roughness());
    const float index_of_refraction = hit.mat->ior;
    auto r = w_o;
    auto v = w_i;
    auto nShiny = exponent;
    auto correction = ((nShiny + 1)/(2*PI));
    //auto phong = pow(max(dot(r, v), 0.f), nShiny);
    auto h = normalize(v+r);
    auto blinnPhong = pow(max(dot(hit.N, h), 0.f), nShiny);
    auto correctedPhong = correction * blinnPhong;
    if(correctedPhong >1){
        auto i = 1;
    }
    
    //auto specColor = hit.Le();
    auto specColor = hit.albedo();
    //auto specColor = vec3(1,1,1);
    auto phongedColor = specColor*correctedPhong;

    auto omega = dot(hit.N, w_i);
    auto schlick = fresnel_schlick(cos(omega), index_of_refraction);
    auto fresnelColor = phongedColor * schlick;
    //return fresnelColor;
    return fresnelColor;
}

std::tuple<vec3, vec3, float> SpecularPhong::sample(const SurfaceInteraction& hit, const vec3& w_o, const vec2& sample) const {
    throw std::runtime_error("Function not implemented: " + std::string(__FILE__) + ", line: " + std::to_string(__LINE__));
}

float SpecularPhong::pdf(const SurfaceInteraction &hit, const vec3 &w_o, const vec3 &w_i) const {
    throw std::runtime_error("Function not implemented: " + std::string(__FILE__) + ", line: " + std::to_string(__LINE__));
}

// ----------------------------------------------------------------------------------------------
// Microfacet distribution helper functions

// dont think i need this, because the function doesnt get called if the rays are not reflectet somewhere usefull
//inline float x_plus(float a){
//    return a > 0 ? 1.f : 0.f;
//}

inline float GGX_D(const float NdotH, float roughness) {
    // TODO ASSIGNMENT2 (optional)
    // compute the GGX D term here
    //auto xi1 = RNG::uniform<float>();
    //auto xi2 = RNG::uniform<float>();
    auto ag = roughness;
    //auto theta = atan((ag*sqrt(xi1))/sqrt(1-xi1));
    //auto theta = isinf(abs(NdotH)) ? PI/2 : acos(NdotH);
    auto theta = acos(NdotH);
    //auto phi = 2*PI*xi2;
    auto d = (pow(ag, 2))/(PI*pow(cos(theta), 4)*pow(pow(ag, 2)+ pow(tan(theta), 2), 2));
    return d;
}

inline float GGX_G1(const float NdotV, float roughness) {
    // TODO ASSIGNMENT2 (optional)
    // compute the GGX G1 term here
    auto ag = roughness;
    //auto theta = isinf(abs(NdotV)) ? PI/2 : acos(NdotV);
    auto theta = acos(NdotV);
    auto g = 2/(1+sqrt(1+pow(ag, 2)*pow(tan(theta), 2)));
    return g;
}

vec3 GGX_sample(const vec2& sample, float roughness) {
    // TODO ASSIGNMENT2 (optional)
    // implement sampling the GGX distribution here
    // note: return a direction in tangent space
    return vec3(0);
}

inline float GGX_pdf(float D, float NdotH, float HdotV) {
    // TODO ASSIGNMENT2 (optional)
    // compute the microfacet PDF here
    return 0.f;
}

// ----------------------------------------------------------------------------------------------
// Microfacet reflection

vec3 MicrofacetReflection::eval(const SurfaceInteraction &hit, const vec3 &w_o, const vec3 &w_i) const {
    // TODO ASSIGNMENT2
    // evaluate the full microfacet BRDF here, optionally relying on the above functions for the D and G1 terms
    // note: use schlick's approximation for the F term
    const float alpha = hit.roughness();
    const float microfacet = 0.f;
    auto i = w_i;
    auto o = w_o;
    auto n = normalize(hit.N);
    auto hr = normalize(i+o);
    auto omega = dot(hit.N, w_i);
    const float index_of_refraction = hit.mat->ior;
    auto f = fresnel_schlick(cos(omega), index_of_refraction);
    auto m = hr;
    auto g = GGX_G1(dot(i,m), alpha)*GGX_G1(dot(o,m), alpha);
    auto d = GGX_D(dot(n, hr), alpha);
    auto fs = (f*g*d)/(4.f*abs(dot(i,n))*abs(dot(o,n)));
    return coated ? vec3(fs) : hit.albedo() * fs;



    //return coated ? vec3(microfacet) : hit.albedo() * microfacet;
}

std::tuple<vec3, vec3, float> MicrofacetReflection::sample(const SurfaceInteraction& hit, const vec3& w_o, const vec2& sample) const {
    // TODO ASSIGNMENT2
    // importance sample and evaluate this microfacet BRDF
    // set w_i to the sampled (world-space!) direction, pdf to the respective PDF and brdf to the evaluated BRDF
    const vec3 w_i = vec3(0);
    const vec3 brdf = vec3(0);
    const float pdf = 0.f;
    return { brdf, w_i, pdf };
}

float MicrofacetReflection::pdf(const SurfaceInteraction &hit, const vec3 &w_o, const vec3 &w_i) const {
    throw std::runtime_error("Function not implemented: " + std::string(__FILE__) + ", line: " + std::to_string(__LINE__));
}

// ------------------------------------------------
// Microfacet transmission

vec3 MicrofacetTransmission::eval(const SurfaceInteraction& hit, const vec3& w_o, const vec3& w_i) const {
    throw std::runtime_error("Function not implemented: " + std::string(__FILE__) + ", line: " + std::to_string(__LINE__));
}

std::tuple<vec3, vec3, float> MicrofacetTransmission::sample(const SurfaceInteraction& hit, const vec3& w_o, const vec2& sample) const {
    throw std::runtime_error("Function not implemented: " + std::string(__FILE__) + ", line: " + std::to_string(__LINE__));
}

float MicrofacetTransmission::pdf(const SurfaceInteraction& hit, const vec3& w_o, const vec3& w_i) const {
    throw std::runtime_error("Function not implemented: " + std::string(__FILE__) + ", line: " + std::to_string(__LINE__));
}

// -------------------------------------------------------------------------------------------
// Layered

vec3 LayeredSurface::eval(const SurfaceInteraction& hit, const vec3& w_o, const vec3& w_i) const {
    const float F = fresnel_dielectric(abs(dot(hit.N, w_o)), 1.f, hit.mat->ior);
    return mix(diff.eval(hit, w_o, w_i), spec.eval(hit, w_o, w_i), F);
}

std::tuple<vec3, vec3, float> LayeredSurface::sample(const SurfaceInteraction& hit, const vec3& w_o, const vec2& sample) const {
    const float F = fresnel_dielectric(abs(dot(hit.N, w_o)), 1.f, hit.mat->ior);
    vec3 brdf;
    if (sample.x < F) {
        // sample specular
        const vec2 sample_mapped = vec2((F - sample.x) / F, sample.y);
        const auto [specular, w_i, sample_pdf] = spec.sample(hit, w_o, sample_mapped);
        if (!same_hemisphere(hit.Ng, w_i)) return { vec3(0), w_i, 0.f };
        assert(std::isfinite(sample_pdf));
        return { mix(diff.eval(hit, w_o, w_i), specular, F), w_i, mix(diff.pdf(hit, w_o, w_i), sample_pdf, F) };
    } else {
        // sample diffuse
        const vec2 sample_mapped = vec2((sample.x - F) / (1 - F), sample.y);
        const auto [diffuse, w_i, sample_pdf] = diff.sample(hit, w_o, sample_mapped);
        if (!same_hemisphere(hit.Ng, w_i)) return { vec3(0), w_i, 0.f };
        assert(std::isfinite(sample_pdf));
        return { mix(diffuse, spec.eval(hit, w_o, w_i), F), w_i, mix(sample_pdf, spec.pdf(hit, w_o, w_i), F) };
    }
}

float LayeredSurface::pdf(const SurfaceInteraction& hit, const vec3& w_o, const vec3& w_i) const {
    const float F = fresnel_dielectric(abs(dot(hit.N, w_o)), 1.f, hit.mat->ior);
    return mix(diff.pdf(hit, w_o, w_i), spec.pdf(hit, w_o, w_i), F);
}

// ----------------------------------------------------------------------------------------------
// Metal

vec3 MetallicSurface::eval(const SurfaceInteraction& hit, const vec3& w_o, const vec3& w_i) const {
    throw std::runtime_error("Function not implemented: " + std::string(__FILE__) + ", line: " + std::to_string(__LINE__));
}

// ----------------------------------------------------------------------------------------------
// Glass

vec3 GlassSurface::eval(const SurfaceInteraction &hit, const vec3 &w_o, const vec3 &w_i) const {
    throw std::runtime_error("Function not implemented: " + std::string(__FILE__) + ", line: " + std::to_string(__LINE__));
}

std::tuple<vec3, vec3, float> GlassSurface::sample(const SurfaceInteraction& hit, const vec3& w_o, const vec2& sample) const {
    throw std::runtime_error("Function not implemented: " + std::string(__FILE__) + ", line: " + std::to_string(__LINE__));
}

float GlassSurface::pdf(const SurfaceInteraction &hit, const vec3 &w_o, const vec3 &w_i) const {
    throw std::runtime_error("Function not implemented: " + std::string(__FILE__) + ", line: " + std::to_string(__LINE__));
}
