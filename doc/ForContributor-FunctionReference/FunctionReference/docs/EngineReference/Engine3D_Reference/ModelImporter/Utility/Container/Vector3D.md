# Vector 3D Class (`Vector3D.h`)

> Path: `Source/Engine3D/ModelImporter/Utility/Container/Vector3D.h`  
> Type: Source  
> Language: C++  
> Note: Model Importer Module is based on Assimp Library.

This **template class** gives you a container that supports 3D-Vector.

## Public Functions

|                                 |                                                                           |
|:--------------------------------|:--------------------------------------------------------------------------|
| Constructor `noexcept`          | `RtMIVector3D_Template()`                                                 |
| Constructor                     | `RtMIVector3D_Template(Tp _x, Tp _y, Tp _z)`                              |
| Constructor                     | `RtMIVector3D_Template(Tp _xyz)`                                          |
| Constructor `default`           | `RtMIVector3D_Template(const RtMIVector3D_Template &rms)`                 |
| Operator Overload               | `operator*=(const RtMIMatrix3x3_Template<Tp> &mat)`                       |
| Operator Overload               | `operator*=(const RtMIMatrix4x4_Template<Tp> &mat)`                       |
| Operator Overload `Tp`          | `operator[](unsigned int i) const`                                        |
| Operator Overload `Tp`          | `&operator[](unsigned int i)`                                             |
| Operator Overload `bool`        | `operator==(const RtMIVector3D_Template &other) const`                    |
| Operator Overload `bool`        | `operator!=(const RtMIVector3D_Template &other) const`                    |
| Operator Overload `bool`        | `operator<(const RtMIVector3D_Template &other) const`                     |
| `bool`                          | `equal(const RtMIVector3D_Template &other, Tp epsilon = g_epsilon) const` |
| Template Function               | `operator RtMIVector3D_Template<TOther>() const`                          |
| `Tp`                            | `squareLength() const`                                                    |
| `Tp`                            | `length()`                                                                |
| `RtMIVector3D_Template`         | `&normalize()`                                                            |
| `RtMIVector3D_Template`         | `&normalizeSafe()`                                                        |
| `RtMIVector3D_Template` `const` | `symmetricMultiply(const RtMIVector3D_Template &o)`                       |

## Public Members

|      |     |
|:-----|:----|
| `Tp` | `x` |
| `Tp` | `y` |
| `Tp` | `z` |

## Detailed Description

This template class, `RtMIVector3D_Template`, is designed to provide a flexible and efficient container for 3D vectors.
The class is templated to allow the use of different numeric types (e.g., `float`, `double`, `int`), making it versatile
for Model Importing.  

It's a key part of Model Importing module. It **should NOT use outside of Model Importing Module**.