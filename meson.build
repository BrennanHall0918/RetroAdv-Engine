project(
  'retroadv',
  'c',
  default_options: [
    'c_std=c23',
    'b_sanitize=address',
    'werror=true',
  ])

add_project_arguments('-g3', language: 'c')
sdl = dependency('SDL3', method:'cmake')
incdir = include_directories('include')
executable(
    'retroadv',
    [
        'src/main.c',
    ],
    include_directories : incdir,
    dependencies:[
        sdl,   
    ]
)
