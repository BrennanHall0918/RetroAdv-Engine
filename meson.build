project(
  'retroadv',
  'c',
  default_options: [
    'c_std=c18',
    'b_sanitize=address',
    'werror=true',
  ])

add_project_arguments('-g3', language: 'c')
sdl = dependency('SDL3', method:'cmake')
sdl_image = dependency('SDL3_image')
incdir = include_directories('include')
executable(
    'retroadv',
    [
        'src/main.c',
        'src/eventloop.c',
        'src/drawloop.c',
    ],
    include_directories : incdir,
    dependencies:[
        sdl_image,
        sdl,   
    ]
)