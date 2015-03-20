local ffi = require 'ffi'


ffi.cdef[[
/*!
 * \addtogroup vi_defines List of defines used for the VIDEO subsystem
 * @{
 */


enum { VI_DISPLAY_PIX_SZ = 2 };		/*!< multiplier to get real pixel size in bytes */

/*!
 * \addtogroup vi_modetypedef VIDEO mode types
 * @{
 */

enum { VI_INTERLACE = 0 };		/*!< Video mode INTERLACED. */
enum { VI_NON_INTERLACE = 1 };		/*!< Video mode NON INTERLACED */
enum { VI_PROGRESSIVE = 2 };		/*!< Video mode PROGRESSIVE. Special mode for higher quality */

/*!
 * @}
 */


/*!
 * \addtogroup vi_standardtypedef VIDEO standard types
 * @{
 */

enum { VI_NTSC = 0 };		/*!< Video standard used in North America and Japan */
enum { VI_PAL = 1 };		/*!< Video standard used in Europe */
enum { VI_MPAL = 2 };		/*!< Video standard, similar to NTSC, used in Brazil */
enum { VI_DEBUG = 3 };		/*!< Video standard, for debugging purpose, used in North America and Japan. Special decoder needed */
enum { VI_DEBUG_PAL = 4 };		/*!< Video standard, for debugging purpose, used in Europe. Special decoder needed */
enum { VI_EURGB60 = 5 };		/*!< RGB 60Hz, 480 lines mode (same timing and aspect ratio as NTSC) used in Europe */

/*!
 * @}
 */


enum { VI_XFBMODE_SF = 0 };
enum { VI_XFBMODE_DF = 1 };


/*!
 * \addtogroup vi_fielddef VIDEO field types
 * @{
 */

enum { VI_FIELD_ABOVE = 1 };		/*!< Upper field in DS mode */
enum { VI_FIELD_BELOW = 0 };		/*!< Lower field in DS mode */

/*!
 * @}
 */


// Maximum screen space
enum { VI_MAX_WIDTH_NTSC = 720 };
enum { VI_MAX_HEIGHT_NTSC = 480 };

enum { VI_MAX_WIDTH_PAL = 720 };
enum { VI_MAX_HEIGHT_PAL = 574 };

enum { VI_MAX_WIDTH_MPAL = 720 };
enum { VI_MAX_HEIGHT_MPAL = 480 };

enum { VI_MAX_WIDTH_EURGB60 = VI_MAX_WIDTH_NTSC };
enum { VI_MAX_HEIGHT_EURGB60 = VI_MAX_HEIGHT_NTSC };


extern GXRModeObj TVNtsc240Ds;				/*!< Video and render mode configuration for 240 lines,singlefield NTSC mode */
extern GXRModeObj TVNtsc240DsAa;			/*!< Video and render mode configuration for 240 lines,singlefield,antialiased NTSC mode */
extern GXRModeObj TVNtsc240Int;				/*!< Video and render mode configuration for 240 lines,interlaced NTSC mode */
extern GXRModeObj TVNtsc240IntAa;			/*!< Video and render mode configuration for 240 lines,interlaced,antialiased NTSC mode */
extern GXRModeObj TVNtsc480Int;				/*!< Video and render mode configuration for 480 lines,interlaced NTSC mode */
extern GXRModeObj TVNtsc480IntDf;			/*!< Video and render mode configuration for 480 lines,interlaced,doublefield NTSC mode */
extern GXRModeObj TVNtsc480IntAa;			/*!< Video and render mode configuration for 480 lines,interlaced,doublefield,antialiased NTSC mode */
extern GXRModeObj TVNtsc480Prog;            /*!< Video and render mode configuration for 480 lines,progressive,singlefield NTSC mode */
extern GXRModeObj TVNtsc480ProgSoft;
extern GXRModeObj TVNtsc480ProgAa;
extern GXRModeObj TVMpal480IntDf;			/*!< Video and render mode configuration for 480 lines,interlaced,doublefield,antialiased MPAL mode */
extern GXRModeObj TVMpal480IntAa;
extern GXRModeObj TVMpal240Ds;
extern GXRModeObj TVMpal240DsAa;
extern GXRModeObj TVPal264Ds;				/*!< Video and render mode configuration for 264 lines,singlefield PAL mode */
extern GXRModeObj TVPal264DsAa;				/*!< Video and render mode configuration for 264 lines,singlefield,antialiased PAL mode */
extern GXRModeObj TVPal264Int;				/*!< Video and render mode configuration for 264 lines,interlaced PAL mode */
extern GXRModeObj TVPal264IntAa;			/*!< Video and render mode configuration for 264 lines,interlaced,antialiased PAL mode */
extern GXRModeObj TVPal524IntAa;			/*!< Video and render mode configuration for 524 lines,interlaced,antialiased PAL mode */
extern GXRModeObj TVPal528Int;				/*!< Video and render mode configuration for 528 lines,interlaced,antialiased PAL mode */
extern GXRModeObj TVPal528IntDf;			/*!< Video and render mode configuration for 264 lines,interlaced,doublefield antialiased PAL mode */
extern GXRModeObj TVPal574IntDfScale;
extern GXRModeObj TVEurgb60Hz240Ds;
extern GXRModeObj TVEurgb60Hz240DsAa;
extern GXRModeObj TVEurgb60Hz240Int;
extern GXRModeObj TVEurgb60Hz240IntAa;
extern GXRModeObj TVEurgb60Hz480Int;
extern GXRModeObj TVEurgb60Hz480IntDf;
extern GXRModeObj TVEurgb60Hz480IntAa;
extern GXRModeObj TVEurgb60Hz480Prog;
extern GXRModeObj TVEurgb60Hz480ProgSoft;
extern GXRModeObj TVEurgb60Hz480ProgAa;
]]

local video_types = {}
function video_types.VIDEO_PadFramebufferWidth(width) return ffi.cast('u16', bit.band(ffi.cast('u16', width) + 15, bit.bnot(15))) end				--/*!< macro to pad the width to a multiple of 16 */

function video_types.VI_TVMODE(fmt, mode) return bit.lshift(fmt, 2) + mode end
function video_types.VI_TVMODE_NTSC_INT		return VI_TVMODE(VI_NTSC,        VI_INTERLACE) end
function video_types.VI_TVMODE_NTSC_DS		return VI_TVMODE(VI_NTSC,        VI_NON_INTERLACE) end
function video_types.VI_TVMODE_NTSC_PROG		return VI_TVMODE(VI_NTSC,        VI_PROGRESSIVE) end
function video_types.VI_TVMODE_NTSC_PROG_DS	return VI_TVMODE(VI_NTSC,        bit.bor(VI_PROGRESSIVE,VI_NON_INTERLACE)) end

function video_types.VI_TVMODE_PAL_INT		return VI_TVMODE(VI_PAL,         VI_INTERLACE) end
function video_types.VI_TVMODE_PAL_DS		return VI_TVMODE(VI_PAL,         VI_NON_INTERLACE) end

function video_types.VI_TVMODE_EURGB60_INT	return VI_TVMODE(VI_EURGB60,     VI_INTERLACE) end
function video_types.VI_TVMODE_EURGB60_DS	return VI_TVMODE(VI_EURGB60,     VI_NON_INTERLACE) end
function video_types.VI_TVMODE_EURGB60_PROG	return VI_TVMODE(VI_EURGB60,     VI_PROGRESSIVE) end
function video_types.VI_TVMODE_EURGB60_PROG_DSreturn VI_TVMODE(VI_EURGB60,     bit.bor(VI_PROGRESSIVE,VI_NON_INTERLACE)) end

function video_types.VI_TVMODE_MPAL_INT		return VI_TVMODE(VI_MPAL,        VI_INTERLACE) end
function video_types.VI_TVMODE_MPAL_DS		return VI_TVMODE(VI_MPAL,        VI_NON_INTERLACE) end

function video_types.VI_TVMODE_DEBUG_INT		return VI_TVMODE(VI_DEBUG,       VI_INTERLACE) end

function video_types.VI_TVMODE_DEBUG_PAL_INT	return VI_TVMODE(VI_DEBUG_PAL,   VI_INTERLACE) end
function video_types.VI_TVMODE_DEBUG_PAL_DS	return VI_TVMODE(VI_DEBUG_PAL,   VI_NON_INTERLACE) end

return setmetatable(video_types, {__index=ffi.C})	-- can I do this or do I need a method?