#ifndef RETO_COLOR_RGBA_H
#define RETO_COLOR_RGBA_H

namespace Retoccilus::Engine3D::ModelImporter {
    template <typename Tp> class RtMIColorRGBA_Template {
    public:
        RtMIColorRGBA_Template() noexcept : r(), g(), b(), a() {
        }
        RtMIColorRGBA_Template(Tp _r, Tp _g, Tp _b, Tp _a) : r(_r), g(_g), b(_b), a(_a) {
        }
        explicit RtMIColorRGBA_Template(Tp _r) : r(_r), g(_r), b(_r), a(_r) {
        }
        RtMIColorRGBA_Template(const RtMIColorRGBA_Template &o) = default;

        const RtMIColorRGBA_Template &operator+=(const RtMIColorRGBA_Template &o);
        const RtMIColorRGBA_Template &operator-=(const RtMIColorRGBA_Template &o);
        const RtMIColorRGBA_Template &operator*=(Tp f);
        const RtMIColorRGBA_Template &operator/=(Tp f);

        bool operator==(const RtMIColorRGBA_Template &other) const;
        bool operator!=(const RtMIColorRGBA_Template &other) const;
        bool operator<(const RtMIColorRGBA_Template &other) const;

        inline Tp operator[](unsigned int i) const;
        inline Tp &operator[](unsigned int i);

        [[nodiscard]] inline bool isBlack() const;

        Tp r;
        Tp g;
        Tp b;
        Tp a;
    };
} // namespace Retoccilus::Engine3D::ModelImporter

#endif // RETO_COLOR_RGBA_H