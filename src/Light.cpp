#include "Light.hpp"
#include "World.hpp"

Light::Light(Tuple position, Color intensity) : position_(position), intensity_(intensity) {}

float Light::intensity_at(const Tuple &p, const World &w) const {
    return 0.0;
}

Tuple Light::position() const {
    return position_;
}

Color Light::intensity() const {
    return intensity_;
}

size_t Light::n_samples() const {
    return n_samples_;
}

Color Lighting(const Material &m, const ShapeConstPtr &s, const LightPtr &light, const Tuple &p, const Tuple &eyev, const Tuple &normalv, float light_intensity) {
    // Combine the surface color with the light's color/intensity
    Color effective_color = m.color_at(s, p) * light->intensity();

    // Compute the ambient contribution
    Color ambient = effective_color * m.get_ambient();

    if (equal(light_intensity, 0.0f))
        return ambient;


    Color sum = Color::black();
    for (auto sample : light->sample_cache_) {
        Color diffuse, specular;
        // Find the direction to the light source
        Tuple lightv = (sample - p).normalize();

        // light_dot_normal represents the cosine of the angle between the # light vector
        // and the normal vector. A negative number means the # light is on the other
        // side of the surface.
        const float light_dot_normal = lightv.dot(normalv);

        if (light_dot_normal < 0.0f) {
            diffuse = Color::black();
            specular = Color::black();
        } else {
            // Compute the diffuse contribution
            diffuse = (effective_color * m.get_diffuse() * light_dot_normal);

            // Reflect_dot_eye represents the cosine of the angle between the reflection vector and the eye vector. A negative number means the # light reflects away from the eye.
            const Tuple reflectv = (-lightv).reflect(normalv);
            const float reflect_dot_eye = reflectv.dot(eyev);

            if (reflect_dot_eye <= 0.0f)
                specular = Color::black();
            else {
                // Compute the specular contribution
                const float factor = powf(reflect_dot_eye, m.get_shininess());
                specular = light->intensity() * m.get_specular() * factor;
            }
        }
        sum = sum + diffuse;
        sum = sum + specular;
    }
    return ambient + (sum / light->n_samples()) * light_intensity;
}

bool Light::is_equal(const Light &rhs) const {
    return intensity_ == rhs.intensity_ &&
           position_ == rhs.position_;
}

bool Light::operator==(const Light &rhs) const {
    return is_equal(rhs);
}
