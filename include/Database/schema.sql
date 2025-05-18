CREATE TABLE IF NOT EXISTS tile_sheet (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  path TEXT,
  name TEXT,
  width INTEGER,
  height INTEGER,
  tile_size INTEGER
  ) STRICT;
  
CREATE TABLE IF NOT EXISTS tile (
   id INTEGER PRIMARY KEY AUTOINCREMENT,
   tile_sheet_id INTEGER,
   tile_offset INTEGER,
   FOREIGN KEY (tile_sheet_id) REFERENCES tile_sheet (id)
   ) STRICT;
   
CREATE TABLE IF NOT EXISTS tile_tag (
   id INTEGER PRIMARY KEY AUTOINCREMENT,
   tag TEXT,
   tile_id INTEGER,
   FOREIGN KEY (tile_id) REFERENCES tile (id)
   ) STRICT;

CREATE TABLE IF NOT EXISTS sprite_sheet (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  path TEXT,
  name TEXT,
  width INTEGER,
  height INTEGER
) STRICT;

CREATE TABLE IF NOT EXISTS entity_type (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  name TEXT,
  maximum_speed REAL,
  acceleration REAL,
  width INTEGER,
  height INTEGER,
) STRICT;

CREATE TABLE IF NOT EXISTS entity_tag (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  tag TEXT,
  duration INTEGER,
  entity_id INTEGER,
  FOREIGN KEY (entity_id) REFERENCES entity_type (id)
) STRICT;

CREATE TABLE IF NOT EXISTS entity_sprite_sheet (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  entity_id INTEGER,
  sprite_sheet_id INTEGER,
  tag_id INTEGER,
  FOREIGN KEY (entity_id) REFERENCES entity_type (id),
  FOREIGN KEY (sprite_sheet_id) REFERENCES sprite_sheet (id),
  FOREIGN KEY (tag_id) REFERENCES tag (id)
) STRICT;




   
   
     









