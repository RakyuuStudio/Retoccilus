## System Requirement of Running Retoccilus Renderer

Because Vulkan and all the renderer needs high-performance components, so
I write this guide that help you to build a computer that fit the requirements.

#### Minimal System Requirement

1. CPU (Central Processing Unit):
    - For Intel: At least 2 Cores and 4 Threads and 2.5 GHz. For example: Intel(R) Core(TM) i3-8100.
    - For AMD: At least 2 Cores and 4 Threads and 2.5 GHz. For example: AMD(R) Ryzen(TM) 3 2200U.
2. GPU (Graphical Processing Unit)
    - If you want Ray Tracing:
        - NVIDIA: RTX 20-Series at least. For example: NVIDIA(R) GeForce(TM) RTX 2060.
        - AMD: Radeon RX 6000 Series at least. For example: AMD(R) Radeon(TM) RX 6800.
        - Intel: Arc A Series at least. For example: Intel(R) Arc(TM) A310.
    - If you don't want Ray Tracing:
        - NVIDIA: GTX 1650 or above.
        - AMD: Radeon RX 580 at least. (P.S. I don't sure the performance on RX 580.)
        - Intel: Arc A310 or above.
    - About Driver:
        - Must the newest driver version. Make sure the driver support Vulkan Ray Tracing Extension
          if you want Ray Tracing.
    - About VRAM:
        - At least 4GB.
3. RAM (Random Access Memory)
    - At least 8GB.
4. Operating System (OS)
    - Windows: Windows 10 or above.
    - Linux: Support Vulkan Ray Tracing Versions.
    - macOS: Not recommended but I will use MoltenVK to make sure macOS works well with Vulkan.

#### Recommended System Requirement

1. CPU (Central Processing Unit):
    - For Intel: At least 4 Cores 8 Threads or above. For example: Intel(R) Core(TM) i5-12400.
    - For AMD: At least 4 Cores 8 Threads or above. For example: AMD(R) Ryzen(TM) 7 5800H.
2. GPU (Graphical Processing Unit):
    - If you want Ray Tracing:
        - NVIDIA: RTX 30-Series at least.
        - AMD: RX 6000 Series at least.
        - Intel: Arc A Series at least.
    - If you don't want Ray Tracing:
        - NVIDIA: RTX 20-Series at least. 
        - AMD: RX 6000 Series at least.
        - Intel: Arc A Series at least.
    - About Driver:
        - Must the newest driver version. Make sure the driver support Vulkan Ray Tracing Extension
          if you want Ray Tracing.
    - About VRAM:
        - At least 8GB.
3. RAM (Random Access Memory)
    - At least 16 GB.
4. Operating System (OS)
    - Windows: Windows 10 or above.
    - Linux: Support Vulkan Ray Tracing Versions.
    - macOS: Not recommended but I will use MoltenVK to make sure macOS works well with Vulkan.