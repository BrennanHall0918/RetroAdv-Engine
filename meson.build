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
os_srces = []
test_os_srces = []

# Dependencies
if os == 'windows'
  meson.add_install_script('post_install.fish')
  # We are msys2's clang64 env to combine the flexibility of msys2
  # with a build environment that actually supports asan... Unfortunately, we lose c23 because Window.
  #
  # dynamic asan links libstc++.dll, let's link statically to avoid having to vendor the dll.
  add_project_arguments('-static-libasan', language: 'c')
  add_global_link_arguments('-mwindows', language: 'c')
  test_os_srces += 'src/tests/windowsmain.c'
endif

sdl = dependency('SDL3', method:'cmake')
sdl_image = dependency('SDL3_image', method: 'cmake')
sqlite = dependency('sqlite3')
dependencies += [sdl, sdl_image, sqlite]

incdir = include_directories('include')

# Executable
list_test = executable(
  'list_test',
  [
    'src/lists/list.c',
    'src/tests/list.c',
  ] + test_os_srces,
  include_directories : incdir,
  install: false)

test('list_test', list_test) 
executable(
  'retroadv',
  [
      'src/main.c',
      'src/eventloop.c',
      'src/drawloop.c',
      'src/entity.c',
      'src/utils.c',
  ],
  include_directories : incdir,
  dependencies: dependencies,
  install: true,
  win_subsystem: 'console'
)
