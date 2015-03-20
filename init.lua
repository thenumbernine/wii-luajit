local ffi = require 'ffi'
local bit = require 'bit'
local system = require 'ffi.ogc.system'
local video = require 'ffi.ogc.video'
local consol = require 'ffi.ogc.consol'
local gx = require 'ffi.ogc.gx'
local wpad = require 'ffi.wiiuse.wpad'
local C = require 'ffi.C'

-- ogc/system.h
function system.MEM_K0_TO_K1(x)
	return ffi.cast('void*', ffi.cast('u32', x) + (system.SYS_BASE_UNCACHED - system.SYS_BASE_CACHED))
end

--[[ single-buffer console
video.VIDEO_Init()
wpad.WPAD_Init()
local rmode = video.VIDEO_GetPreferredMode(nil)
local xfb = MEM_K0_TO_K1(system.SYS_AllocateFramebuffer(rmode))
consol.CON_Init(xfb, 20, 20, rmode[0].fbWidth, rmode[0].xfbHeight, rmode[0].fbWidth * video.VI_DISPLAY_PIX_SZ)
video.VIDEO_Configure(rmode)
video.VIDEO_SetNextFramebuffer(xfb)
video.VIDEO_SetBlack(0)
video.VIDEO_Flush()
video.VIDEO_WaitVSync()
if bit.band(rmode[0].viTVMode, video.VI_NON_INTERLACE) ~= 0 then video.VIDEO_WaitVSync() end
C.printf('\27[2;0H')
--]]


--[=[ double-buffer neheGX demo

local DEFAULT_FIFO_SIZE = 256 * 1024

video.VIDEO_Init()
wpad.WPAD_Init()

local rmode = VIDEO_GetPreferredMode(nil)

local fb = 1
local frameBuffer = {
	MEM_K0_TO_K1(system.SYS_AllocateFramebuffer(rmode)),
	MEM_K0_TO_K1(system.SYS_AllocateFramebuffer(rmode)),
}

video.VIDEO_Configure(rmode)
video.VIDEO_SetNextFramebuffer(frameBuffer[fb])
video.VIDEO_SetBlack(0)
video.VIDEO_Flush()
video.VIDEO_WaitVSync()
if bit.band(rmode[0].viTVMode, video.VI_NON_INTERLACE) ~= 0 then video.VIDEO_WaitVSync() end

local gp_fifo = C.memalign(32, DEFAULT_FIFO_SIZE)
C.memset(gp_fifo, 0, DEFAULT_FIFO_SIZE)

gx.GX_Init(gp_fifo, DEFAULT_FIFO_SIZE)

local background = ffi.new('GXColor')
background.r = 0
background.g = 0
background.b = 0
background.a = 0xff

gx.GX_SetCopyClear(background, 0x00ffffff)	-- probably buggy because it does pass-struct-by-reference
gx.GX_SetViewport(0,0,rmode[0].fbWidth,rmode[0].efbHeight,0,1)
local yscale = gx.GX_GetYScaleFactor(rmode[0].efbHeight,rmode[0].xfbHeight)
local xfbHeight = gx.GX_SetDispCopyYScale(yscale)
gx.GX_SetScissor(0,0,rmode[0].fbWidth,rmode[0].efbHeight);
gx.GX_SetDispCopySrc(0,0,rmode[0].fbWidth,rmode[0].efbHeight);
gx.GX_SetDispCopyDst(rmode[0].fbWidth,xfbHeight);
gx.GX_SetCopyFilter(rmode[0].aa,rmode[0].sample_pattern,gx.GX_TRUE,rmode[0].vfilter);
gx.GX_SetFieldMode(rmode[0].field_rendering, rmode[0].viHeight == 2 * rmode[0].xfbHeight and gx.GX_ENABLE or gx.GX_DISABLE)

--]=]

C.printf('\27[2;0H')
print("Testing")

local t = os.time()
local lastPressed
while os.time() < t + 60 do
	wpad.WPAD_ScanPads()
	local pressed = wpad.WPAD_ButtonsDown(0)
	if bit.band(pressed, wpad.WPAD_BUTTON_HOME) ~= 0 then
		print("Goodbye")
		break
	else
		if pressed ~= lastPressed then
			print('buttons down: '..pressed)
			lastPressed = pressed
		end
	end
	video.VIDEO_WaitVSync()
end