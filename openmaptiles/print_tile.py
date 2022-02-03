####################################################################################################

from typing import Iterable
import sqlite3
import gzip

from betterproto import which_one_of

from vector_tile import Tile, TileGeomType

####################################################################################################

def query(conn: sqlite3.Connection, sql: str, params: list) -> Iterable[tuple]:
    cursor = conn.cursor()
    cursor.execute(sql, params)
    yield from cursor

####################################################################################################

def pair_iter(alist):
    for i in range(0, len(alist), 2):
        yield alist[i:i+2]

####################################################################################################

def decode_command(integer):
    command_id = integer & 0x7
    count = integer >> 3
    match command_id:
        case 1:
            return 'MoveTo', 2, count
        case 2:
            return 'LineTo', 2, count
        case 7:
            return 'ClosePath', 0, count*0

####################################################################################################

def decode_value(integer):
    return ((integer >> 1) ^ (-(integer & 1)))

####################################################################################################

def print_tile(data: bytes, info: str) -> None:
    try:
        tile_raw = gzip.decompress(data)
        gzipped_size = len(data)
        info = 'Tile ' + info
    except gzip.BadGzipFile:
        tile_raw = data
        gzipped_size = len(gzip.compress(data))
        info = 'Uncompressed tile ' + info
    tile = Tile().parse(tile_raw)
    print(f'{info} size={len(tile_raw):,} bytes, gzipped={gzipped_size:,} bytes, {len(tile.layers)} layers')
    for layer in tile.layers:
        print()
        print(f"Layer {layer.version} / {layer.name}")
        print(f"  features #{len(layer.features)}")
        print(f"  keys #{len(layer.keys)}")
        print(f"  values #{len(layer.values)}")
        for feature in layer.features:
            type_name = TileGeomType(feature.type).name
            print(f"  id:{feature.id} type:{feature.type}/{type_name} tags:#{len(feature.tags)} geometry:#{len(feature.geometry)}")
            print(f"    tags:#{feature.tags}")
            tags = {
                layer.keys[key]: which_one_of(layer.values[value], 'val')
                for key, value in pair_iter(feature.tags)
            }
            print(f"      {tags}")
            print(f"    geometry:#{feature.geometry}")
            i = 0
            while i < len(feature.geometry):
                command, pcount, count = decode_command(feature.geometry[i])
                i += 1
                print(f"     {i} {command} {pcount}*{count}")
                # for j in range(pcount*count):
                #     value = decode_value(feature.geometry[i])
                #     i += 1
                #     print(f"       {i}/{j} {value}")
                for j in range(count):
                    dx = decode_value(feature.geometry[i])
                    dy = decode_value(feature.geometry[i+1])
                    i += 2
                    print(f"       {i}/{j} {dx} {dy}")

####################################################################################################

def show_tile(path, zoom, x, y):
    with sqlite3.connect(path) as conn:
        sql = 'SELECT tile_data FROM tiles WHERE zoom_level=? AND tile_column=? AND tile_row=?'
        for row in query(conn, sql, [zoom, x, y]):
            print_tile(row[0], f'{zoom}/{x}/{y}')
            break
        else:
            print(f'Tile {zoom}/{x}/{y} not found')

####################################################################################################

if __name__ == '__main__':
    path = 'maptiler-osm-2017-07-03-v3.6.1-france_rhone-alpes.mbtiles'
    show_tile(path, 14, 8519, 10589)
