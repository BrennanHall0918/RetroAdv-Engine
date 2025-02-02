project('retroadv', 'c')
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
