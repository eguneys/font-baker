import { parse } from './parse_out.js'
import fs from 'fs'

import { Packer } from 'aset'


fs.readFile('out.data', (err, data) => {
  let _ = parse(data)

  let packer = new Packer()

  let colors = _.colors.map((color, i) => {
    let image = {
      width: color.width,
      height: color.height, 
      pixels: color.buff
    }


    let pack = packer.add(image)

    return {
      glyph: color.glyph,
      pack
    }
  })

  packer.pack()

  colors = colors.map(({ glyph, pack }) => ({
    glyph,
    frame: pack.frame,
    packed: pack.packed
  }))

  let res = {
    kernings: _.kernings,
    meta: _.meta,
    chars: _.chars,
    glyphs: _.glyphs,
    colors
  }


  fs.writeFileSync('./out_0.png', packer.pages[0].png_buffer)
  fs.writeFileSync('./out_0.json', JSON.stringify(res))

  console.log('content written.')

})
