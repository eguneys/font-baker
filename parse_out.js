function fffffff(buffer) {

  return [...buffer].flatMap((_, i) => i % 2 === 1 ? [String.fromCharCode(_)] : []).join('')

}


export function parse(data) {

  let i = 0

  function _long(skip = 12) {
    let res = data.readInt32LE(i)
    i += 4

    i += skip
    return res
  }

  function _float() {
    let res = data.readFloatLE(i)
    i+= 4
    return res
  }

  function Kerning() {

    let glyph1 = _long()
    let glyph2 = _long()
    let kerning = _float()
    i += 12

    return {
      glyph1,
      glyph2,
      kerning
    }
  }
  function Meta() {

    let scale = _float()
    i += 12
    let descent = _long()
    let ascent = _long()
    let line_gap = _long()
    let name_length = _long()
    let name = fffffff(data.slice(i, i + name_length))
    i += name_length * 4
    
    return {
      scale,
      descent,
      ascent,
      line_gap,
      name
    }
  }
  function CharKK() {

    let _glyph = _long()

    let _i = i
    let glyph = _long(0)
    let width = _long(0)
    let height = _long(0)
    let advance = _float()
    let offset_x = _float()
    let offset_y = _float()
    let scale = _float()
    let has_glyph = _long()
    i = _i + 32 * 4

    return {
      glyph,
      width,
      height,
      advance,
      offset_x,
      offset_y,
      scale,
      has_glyph
    }
  }
  function GlyphKK() {
    let glyph = _long()
    let codepoint = _long()

    return {
      glyph,
      codepoint
    }
  }
  function ColorKK() {

    let glyph = _long()

    let width = _long()
    let height = _long()

    let length = _long()

    let _i = i

    let buff = data.slice(i, i + length * 4)

    i = _i + length * 4

    return {
      glyph,
      length,
      width,
      height,
      buff
    }

  }




  function chunk() {

    let type = _long()
    let c

    switch (type) {
      case 3:
        c = Kerning()
        break;
      case 4:
        c = Meta()
        break;
      case 5:
        c = CharKK()
        break;
      case 6:
        c = GlyphKK()
        break;
      case 7:
        c = ColorKK()
        break;
    }

    return {
      type,
      data: c
    }
  }
  
  let chunks = []

  while (i < data.length) {
    chunks.push(chunk())
  }


  let kernings = chunks.filter(_ => _.type === 3).map(_ => _.data)
  let meta = chunks.filter(_ => _.type === 4).map(_ => _.data)[0]
  let chars = chunks.filter(_ => _.type === 5).map(_ => _.data)
  let glyphs = chunks.filter(_ => _.type === 6).map(_ => _.data)
  let colors = chunks.filter(_ => _.type === 7).map(_ => _.data)

  return {
    kernings,
    meta,
    chars,
    glyphs,
    colors
  }

}
