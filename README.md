# easywasi

A working, zero-dependency implementation of WASI preview1, with lots of filesystem options. It will work in places other than a browser, but that is the primary target. It's easy, light, simple, effortless to modify/extend, and should work with any WASI-enabled wasm and any modern js runtime.

If you need framebuffer/audio/input WASI devices, check out [web-zen-dev](https://github.com/konsumer/web-zen-dev).

## install

Install into your bundled project with `npm i @easywasm/wasi`. You can also use it on the web, directly from cdn:

```html
<script type="module">
  import WasiPreview1 from 'https://esm.sh/@easywasm/wasi'
</script>
```

And I like to use sourcemaps:

```html
<script type="importmap">
  {
    "imports": {
      "@easywasm/wasi": "https://esm.sh/@easywasm/wasi",
      "fflatefs": "https://easywasm.github.io/wasi/fflatefs.js",
      "fflate": "https://esm.sh/fflate/esm/browser.js"
    }
  }
</script>
<script type="module">
  import { WasiPreview1 } from '@easywasm/wasi'
</script>
```

## usage

You can use it without a filesystem, like this:

```js
import WasiPreview1 from '@easywasm/wasi'

const wasi_snapshot_preview1 = new WasiPreview1()

const {
  instance: { exports }
} = await WebAssembly.instantiateStreaming(fetch('example.wasm'), {
  wasi_snapshot_preview1
  // your imports here
})

wasi_snapshot_preview1.start(exports)
```

To really unlock it's power, though, give it an `fs` instance.

I used to use [zenfs](https://github.com/zen-fs/core), but it seems broken every time I try, now. I think they intend that you use packaging and stuff, but I just want to import functional things from the web/cdn. In my opinion it's non-functional.

Now, i use a much simpler approach:

```js
import WasiPreview1 from 'https://esm.sh/@easywasm/wasi'
import fflatefs from 'fflatefs'

const fs = await fflatefs('fs.zip')
const wasi_snapshot_preview1 = new WasiPreview1({ fs })

const {
  instance: { exports }
} = await WebAssembly.instantiateStreaming(fetch('example.wasm'), {
  wasi_snapshot_preview1
  // your imports here
})

wasi_snapshot_preview1.start(exports)
```

This implements the bare-min `statSync`/`readFileSync`. If you want more functionality, implement more, but that is all you really need to read files.

Have a look in [example](docs) to see how I fit it all together.

Keep in mind, you can easily override every function yourself, too, like if you want to implement the socket-API, which is the only thing I left out:

```js
import { defs, WasiPreview1 } from '@easywasm/wasi'

class WasiPreview1WithSockets extends WasiPreview1 {
  constructor(options = {}) {
    super(options)
    // do something with options to setup socket
  }

  // obviously implement these however
  sock_accept(fd, flags) {
    return defs.ERRNO_NOSYS
  }
  sock_recv(fd, riData, riFlags) {
    return defs.ERRNO_NOSYS
  }
  sock_send(fd, siData, riFlags) {
    return defs.ERRNO_NOSYS
  }
  sock_shutdown(fd, how) {
    return defs.ERRNO_NOSYS
  }
}

// usage
const wasi_snapshot_preview1 = new WasiPreview1WithSockets({ fs })

const {
  instance: { exports }
} = await WebAssembly.instantiateStreaming(fetch('example.wasm'), {
  wasi_snapshot_preview1
})
wasi_snapshot_preview1.start(exports)
```

Have a look at [WasiPreview1](./src/easywasi.js) to figure out how to implement it, if you want things to work differently.

## inspiration

- [this article](https://dev.to/ndesmic/building-a-minimal-wasi-polyfill-for-browsers-4nel) has some nice initial ideas
- [this article](https://twdev.blog/2023/11/wasm_cpp_04/) has some good WASI imeplentations
- [browser-wasi-shim](https://github.com/bjorn3/browser_wasi_shim) has a very nice interface, and this is basically the same in JS, but using more extensible filesystem, and I improved a few little things.
