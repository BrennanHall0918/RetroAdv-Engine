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
   tile_id INTEGER,
   FOREIGN KEY (tile_id) REFERENCES tile (id)
   ) STRICT;
   
   
     









