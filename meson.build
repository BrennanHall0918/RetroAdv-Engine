# Project Configuration
project(
  'retroadv',
  'c',
  default_options: [
    'c_std=c18',
    'b_sanitize=address',
    'b_sanitize=undefined',
    'werror=true',
  ])

add_project_arguments('-g3', language: 'c')

os = target_machine.system()
dependencies = []
resources = []

# Dependencies
if os == 'windows'
  meson.add_install_script('post_install.fish')
  # We are msys2's clang64 env to combine the flexibility of msys2
  # with a build environment that actually supports asan... Unfortunately, we lose c23 because Window.
  #
  # dynamic asan links libstc++.dll, let's link statically to avoid having to vendor the dll.
  add_project_arguments('-static-libasan', language: 'c')
  cmake = import('cmake')
  #SDL3
  sdl3_opts = cmake.subproject_options()
  sdl3_opts.set_install(true)
  sdl3_proj = cmake.subproject('sdl3', options: sdl3_opts)
  message('sdl targets:\n - ' + '\n - '.join(sdl3_proj.target_list()))
  sdl = sdl3_proj.dependency('SDL3-shared')
  #sdl_headers = sdl3_proj.dependency('SDL3_Headers')

  # SDL3_image
  sdl3_image_proj = cmake.subproject('sdl3_image', options: sdl3_opts)
  message('sdl_image targets:\n - ' + '\n - '.join(sdl3_image_proj.target_list()))
  sdl_image = sdl3_image_proj.dependency('SDL3_image-shared')

  dependencies += [sdl, sdl_image]
else
  # SDL3
  sdl = dependency('SDL3', method:'cmake')

  # SDL3_image
  sdl_image = dependency('SDL3_image', method: 'cmake')

  dependencies += [sdl, sdl_image]
endif


incdir = include_directories('include')

# Executable
executable(
  'retroadv',
  [
      'src/main.c',
      'src/eventloop.c',
      'src/drawloop.c',
  ],
  include_directories : incdir,
  dependencies: dependencies,
  install: true,
  win_subsystem: 'windows'
)
