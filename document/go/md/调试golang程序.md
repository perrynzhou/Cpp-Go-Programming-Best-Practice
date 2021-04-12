###  调试golang程序

| 作者 | 时间 |QQ技术交流群 |
| ------ | ------ |------ |
| perrynzhou@gmail.com |2020/12/01 |672152841 |

- 安装调试工具

  ```
  $ git clone https://github.com/go-delve/delve
  $ cd delve
  $ go install github.com/go-delve/delve/cmd/dlv
  On Go version 1.16 or later, this command will also work:
  
  $ go install github.com/go-delve/delve/cmd/dlv@latest
  ```

- 调试代码

  ```
  import (
  	"fmt"
  	"time"
  )
  
  func firstGoRoutineA(a chan int) {
  	val := <-a
  	fmt.Println("firstGoRoutineA recevied the data",val)
  }
  func secondGoRoutineB(a chan int) {
  	val := <-a
  	fmt.Println("secondGoRoutineB recevied the data",val)
  }
  
  func main() {
  	ch :=make(chan int)
  	go firstGoRoutineA(ch)
  	go secondGoRoutineB(ch)
  	ch <-3
  	time.Sleep(time.Second*1)
  }
  ```

  

- 调试工具使用

  ```
  
  perrynzhou@mint-os:~/Source/go-example/chan$ dlv debug main.go 
  Type 'help' for list of commands.
  (dlv) br main.main
  Command failed: command not available
  (dlv) break main.main
  Breakpoint 1 set at 0x4b6c93 for main.main() ./main.go:17
  (dlv) break runtime.makechan
  Breakpoint 2 set at 0x404ed3 for runtime.makechan() /home/perrynzhou/App/go/src/runtime/chan.go:71
  (dlv) continue
  > runtime.makechan() /home/perrynzhou/App/go/src/runtime/chan.go:71 (hits goroutine(1):1 total:1) (PC: 0x404ed3)
  Warning: debugging optimized function
      66:         }
      67:
      68:         return makechan(t, int(size))
      69: }
      70:
  =>  71: func makechan(t *chantype, size int) *hchan {
      72:         elem := t.elem
      73:
      74:         // compiler checks this but be safe.
      75:         if elem.size >= 1<<16 {
      76:                 throw("makechan: invalid channel element type")
  (dlv) next
  > runtime.makechan() /home/perrynzhou/App/go/src/runtime/chan.go:72 (PC: 0x404ee1)
  Warning: debugging optimized function
      67:
      68:         return makechan(t, int(size))
      69: }
      70:
      71: func makechan(t *chantype, size int) *hchan {
  =>  72:         elem := t.elem
      73:
      74:         // compiler checks this but be safe.
      75:         if elem.size >= 1<<16 {
      76:                 throw("makechan: invalid channel element type")
      77:         }
  (dlv) next
  > runtime.makechan() /home/perrynzhou/App/go/src/runtime/chan.go:75 (PC: 0x404eea)
  Warning: debugging optimized function
      70:
      71: func makechan(t *chantype, size int) *hchan {
      72:         elem := t.elem
      73:
      74:         // compiler checks this but be safe.
  =>  75:         if elem.size >= 1<<16 {
      76:                 throw("makechan: invalid channel element type")
      77:         }
      78:         if hchanSize%maxAlign != 0 || elem.align > maxAlign {
      79:                 throw("makechan: bad alignment")
      80:         }
  (dlv) n
  > runtime.makechan() /home/perrynzhou/App/go/src/runtime/chan.go:78 (PC: 0x404ef9)
  Warning: debugging optimized function
      73:
      74:         // compiler checks this but be safe.
      75:         if elem.size >= 1<<16 {
      76:                 throw("makechan: invalid channel element type")
      77:         }
  =>  78:         if hchanSize%maxAlign != 0 || elem.align > maxAlign {
      79:                 throw("makechan: bad alignment")
      80:         }
      81:
      82:         mem, overflow := math.MulUintptr(elem.size, uintptr(size))
      83:         if overflow || mem > maxAlloc-hchanSize || size < 0 {
  (dlv) n
  > runtime.makechan() /home/perrynzhou/App/go/src/runtime/chan.go:83 (PC: 0x404f0e)
  Warning: debugging optimized function
      78:         if hchanSize%maxAlign != 0 || elem.align > maxAlign {
      79:                 throw("makechan: bad alignment")
      80:         }
      81:
      82:         mem, overflow := math.MulUintptr(elem.size, uintptr(size))
  =>  83:         if overflow || mem > maxAlloc-hchanSize || size < 0 {
      84:                 panic(plainError("makechan: size out of range"))
      85:         }
      86:
      87:         // Hchan does not contain pointers interesting for GC when elements stored in buf do not contain pointers.
      88:         // buf points into the same allocation, elemtype is persistent.
  (dlv) n
  > runtime.makechan() /home/perrynzhou/App/go/src/runtime/chan.go:93 (PC: 0x404f40)
  Warning: debugging optimized function
      88:         // buf points into the same allocation, elemtype is persistent.
      89:         // SudoG's are referenced from their owning thread so they can't be collected.
      90:         // TODO(dvyukov,rlh): Rethink when collector can move allocated objects.
      91:         var c *hchan
      92:         switch {
  =>  93:         case mem == 0:
      94:                 // Queue or element size is zero.
      95:                 c = (*hchan)(mallocgc(hchanSize, nil, true))
      96:                 // Race detector uses this location for synchronization.
      97:                 c.buf = c.raceaddr()
      98:         case elem.ptrdata == 0:
  (dlv) n
  > runtime.makechan() /home/perrynzhou/App/go/src/runtime/chan.go:98 (PC: 0x404f49)
  Warning: debugging optimized function
      93:         case mem == 0:
      94:                 // Queue or element size is zero.
      95:                 c = (*hchan)(mallocgc(hchanSize, nil, true))
      96:                 // Race detector uses this location for synchronization.
      97:                 c.buf = c.raceaddr()
  =>  98:         case elem.ptrdata == 0:
      99:                 // Elements do not contain pointers.
     100:                 // Allocate hchan and buf in one call.
     101:                 c = (*hchan)(mallocgc(hchanSize+mem, nil, true))
     102:                 c.buf = add(unsafe.Pointer(c), hchanSize)
     103:         default:
  (dlv) next
  > runtime.makechan() /home/perrynzhou/App/go/src/runtime/chan.go:101 (PC: 0x404f54)
  Warning: debugging optimized function
      96:                 // Race detector uses this location for synchronization.
      97:                 c.buf = c.raceaddr()
      98:         case elem.ptrdata == 0:
      99:                 // Elements do not contain pointers.
     100:                 // Allocate hchan and buf in one call.
  => 101:                 c = (*hchan)(mallocgc(hchanSize+mem, nil, true))
     102:                 c.buf = add(unsafe.Pointer(c), hchanSize)
     103:         default:
     104:                 // Elements contain pointers.
     105:                 c = new(hchan)
     106:                 c.buf = mallocgc(mem, elem, true)
  (dlv) next
  > runtime.makechan() /home/perrynzhou/App/go/src/runtime/chan.go:102 (PC: 0x404f79)
  Warning: debugging optimized function
      97:                 c.buf = c.raceaddr()
      98:         case elem.ptrdata == 0:
      99:                 // Elements do not contain pointers.
     100:                 // Allocate hchan and buf in one call.
     101:                 c = (*hchan)(mallocgc(hchanSize+mem, nil, true))
  => 102:                 c.buf = add(unsafe.Pointer(c), hchanSize)
     103:         default:
     104:                 // Elements contain pointers.
     105:                 c = new(hchan)
     106:                 c.buf = mallocgc(mem, elem, true)
     107:         }
  (dlv) n
  > runtime.makechan() /home/perrynzhou/App/go/src/runtime/chan.go:109 (PC: 0x404fa6)
  Warning: debugging optimized function
     104:                 // Elements contain pointers.
     105:                 c = new(hchan)
     106:                 c.buf = mallocgc(mem, elem, true)
     107:         }
     108:
  => 109:         c.elemsize = uint16(elem.size)
     110:         c.elemtype = elem
     111:         c.dataqsiz = uint(size)
     112:         lockInit(&c.lock, lockRankHchan)
     113:
     114:         if debugChan {
  (dlv) n
  > runtime.makechan() /home/perrynzhou/App/go/src/runtime/chan.go:110 (PC: 0x404fb2)
  Warning: debugging optimized function
     105:                 c = new(hchan)
     106:                 c.buf = mallocgc(mem, elem, true)
     107:         }
     108:
     109:         c.elemsize = uint16(elem.size)
  => 110:         c.elemtype = elem
     111:         c.dataqsiz = uint(size)
     112:         lockInit(&c.lock, lockRankHchan)
     113:
     114:         if debugChan {
     115:                 print("makechan: chan=", c, "; elemsize=", elem.size, "; dataqsiz=", size, "\n")
  (dlv) n
  > runtime.makechan() /home/perrynzhou/App/go/src/runtime/chan.go:111 (PC: 0x404fc4)
  Warning: debugging optimized function
     106:                 c.buf = mallocgc(mem, elem, true)
     107:         }
     108:
     109:         c.elemsize = uint16(elem.size)
     110:         c.elemtype = elem
  => 111:         c.dataqsiz = uint(size)
     112:         lockInit(&c.lock, lockRankHchan)
     113:
     114:         if debugChan {
     115:                 print("makechan: chan=", c, "; elemsize=", elem.size, "; dataqsiz=", size, "\n")
     116:         }
  (dlv) n
  > runtime.makechan() /home/perrynzhou/App/go/src/runtime/chan.go:112 (PC: 0x404fcd)
  Warning: debugging optimized function
     107:         }
     108:
     109:         c.elemsize = uint16(elem.size)
     110:         c.elemtype = elem
     111:         c.dataqsiz = uint(size)
  => 112:         lockInit(&c.lock, lockRankHchan)
     113:
     114:         if debugChan {
     115:                 print("makechan: chan=", c, "; elemsize=", elem.size, "; dataqsiz=", size, "\n")
     116:         }
     117:         return c
  (dlv) n
  > runtime.makechan() /home/perrynzhou/App/go/src/runtime/chan.go:117 (PC: 0x404fe5)
  Warning: debugging optimized function
     112:         lockInit(&c.lock, lockRankHchan)
     113:
     114:         if debugChan {
     115:                 print("makechan: chan=", c, "; elemsize=", elem.size, "; dataqsiz=", size, "\n")
     116:         }
  => 117:         return c
     118: }
     119:
     120: // chanbuf(c, i) is pointer to the i'th slot in the buffer.
     121: func chanbuf(c *hchan, i uint) unsafe.Pointer {
     122:         return add(c.buf, uintptr(i)*uintptr(c.elemsize))
  (dlv) p *c
  runtime.hchan {
          qcount: 0,
          dataqsiz: 2,
          buf: unsafe.Pointer(0xc000064060),
          elemsize: 8,
          closed: 0,
          elemtype: *runtime._type {size: 8, ptrdata: 0, hash: 4149441018, tflag: tflagUncommon|tflagExtraStar|tflagNamed|tflagRegularMemory (15), align: 8, fieldAlign: 8, kind: 2, equal: runtime.memequal64, gcdata: *0, str: 847, ptrToThis: 26368},
          sendx: 0,
          recvx: 0,
          recvq: runtime.waitq {
                  first: *runtime.sudog nil,
                  last: *runtime.sudog nil,},
          sendq: runtime.waitq {
                  first: *runtime.sudog nil,
                  last: *runtime.sudog nil,},
          lock: runtime.mutex {
                  lockRankStruct: runtime.lockRankStruct {},
                  key: 0,},}
  ```

  