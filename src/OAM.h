#ifndef NSMB_OAM_INCLUDED
#define NSMB_OAM_INCLUDED

#include "nitro.h"
#include "nsmb/vector.h"

namespace OAM
{

	/* ======== TYPES ======== */

	enum class Flags
	{
		None = 0, // No flags.
		FlipX = 1, // Flips the object horizontally.
		FlipY = 2, // Flips the object vertically.
		Unk4h = 4, // Unknown 0x4.
		FromLast = 8, // Starts rendering the objects from the last entry to the first.
		Prio1 = 0x10, // Draws with priority 1
		Prio2 = 0x20, // Draws with priority 2
		Prio3 = 0x30, // Draws with priority 3
		TranslucentMode = 0x40, // Renders the objects in translucent mode.
		WindowMode = 0x80, // Renders the objects in window mode.
		BitmapMode = 0xC0 // Renders the objects in bitmap mode.
	};

	constexpr Flags operator|(Flags lhs, Flags rhs) {
		return static_cast<Flags>(static_cast<s32>(lhs) | static_cast<s32>(rhs));
	}

	enum class Settings
	{
		None = 0, // No settings.
		Single = 1, // Only draws the first OAM entry.
		NoWrap = 2 // Does not wrap the objects when wrap around is enabled in the level.
	};

	constexpr Settings operator|(Settings lhs, Settings rhs) {
		return static_cast<Settings>(static_cast<s32>(lhs) | static_cast<s32>(rhs));
	}

	enum class CounterFlags
	{
		None = 0, // No flags.
		UpdateShadow = 1, // Updates double the digit count in the array to account for the shadow.
		Unk2h = 2 // Ignores the oam counter attribute table???
	};

	constexpr CounterFlags operator|(CounterFlags lhs, CounterFlags rhs) {
		return static_cast<CounterFlags>(static_cast<s32>(lhs) | static_cast<s32>(rhs));
	}

	/* ======== HELPERS ======== */

	/**
	 * \brief A function that allows to setup OAM directly in an array.
	 * 
	 * \param x The X coordinate of the object.
	 * \param y The Y coordinate of the object.
	 * \param priority The priority of the object.
	 * \param mode The object mode.
	 * \param mosaic If true the object has mosaic effect.
	 * \param effect The effect of the object.
	 * \param shape The size of the object.
	 * \param color The color mode of the object (16/256 colors).
	 * \param charName The tile number.
	 * \param cParam The color parameter.
	 * \param rsParam The affine transformation if an affine effect is specified.
	 * \param attr3 The 3rd attribute (0x8000 = last sprite entry).
	 * 
	 * \return The OAM object.
	 * */
	constexpr GXOamAttr getOBJAttr
	(
		s32 x, s32 y, s32 priority,
		GXOamMode mode, bool mosaic, GXOamEffect effect, GXOamShape shape,
		GXOamColorMode color, s32 charName, s32 cParam, s32 rsParam, s32 attr3
	)
	{
		GXOamAttr oam = { 0 };

		u32 sParams = 0;
		if (effect == GX_OAM_EFFECT_AFFINE || effect == GX_OAM_EFFECT_AFFINE_DOUBLE)
			sParams |= (rsParam << GX_OAM_ATTR01_RS_SHIFT);

		if (mode != GX_OAM_MODE_BITMAPOBJ)
			sParams |= (color << GX_OAM_ATTR01_CM_SHIFT);

		oam.attr01 = static_cast<u32>(
			sParams |
			((x & (GX_OAM_ATTR01_X_MASK >> GX_OAM_ATTR01_X_SHIFT)) << GX_OAM_ATTR01_X_SHIFT) |
			((y & (GX_OAM_ATTR01_Y_MASK >> GX_OAM_ATTR01_Y_SHIFT)) << GX_OAM_ATTR01_Y_SHIFT) |
			(mode << GX_OAM_ATTR01_MODE_SHIFT) |
			(mosaic << GX_OAM_ATTR01_MOSAIC_SHIFT) |
			effect |
			shape
		);

		oam.attr2 = static_cast<u16>(charName | (cParam << GX_OAM_ATTR2_CPARAM_SHIFT) | (priority << GX_OAM_ATTR2_PRIORITY_SHIFT));
		oam._3 = attr3;

		return oam;
	}

	/* ======== COMMON ======== */

	bool updateCounter(GXOamAttr* oamAttrs, GXOamAttr **oamCounterAttrTable, s32 value, s32 digitCount, CounterFlags flags);

	void copyMtx(MtxFx22 *pSrc, MtxFx22 *pDst);

	/* ======== TOP SCREEN ======== */

	// OAM::draw | 0x0200D2A4
	/**
	 * \brief Pushes an object to the top screen render list to be drawn.
	 * 
	 * \param oamAttrs The OAM object attribute array.
	 * \param x The X coordinate relative to the screen of where to draw the objects.
	 * \param y The Y coordinate relative to the screen of where to draw the objects.
	 * \param flags The flags of how to draw the objects.
	 * \param palette The palette shift of which to draw the objects.
	 * \param affineSet The affine set to be used when drawing the objects.
	 * \param settings The object processing settings.
	 * 
	 * \return True if the objects were pushed to the render list successfully, false otherwise.
	 * */
	bool draw(GXOamAttr* oamAttrs, s32 x, s32 y, Flags flags, u8 palette, u8 affineSet, Settings settings);

	// OAM::drawExt | 0x0200D87C
	/**
	 * \brief Pushes an object to the top screen the render list to be drawn. (Extended version)
	 * 
	 * \param oamAttrs The OAM object attribute array.
	 * \param x The X coordinate relative to the screen of where to draw the objects.
	 * \param y The Y coordinate relative to the screen of where to draw the objects.
	 * \param flags The flags of how to draw the objects.
	 * \param palette The palette shift of which to draw the objects.
	 * \param affineSet The affine set to be used when drawing the objects.
	 * \param scale The scale that the objects will be drawn.
	 * \param rot The rotation that the object will be drawn.
	 * \param rotCenter The rotation center.
	 * \param settings The object processing settings.
	 * 
	 * \return True if the objects were pushed to the render list successfully, false otherwise.
	 * */
	bool draw(GXOamAttr* oamAttrs, s32 x, s32 y, Flags flags, u8 palette, u8 affineSet, Vec2* scale, fx16 rot, s16(*rotCenter)[2], Settings settings);

	// OAM::drawMtx | 0x0200CE10
	/**
	 * \brief Pushes an object to the top screen render list to be drawn. (Matrix version)
	 * 
	 * \param oamAttrs The OAM object attribute array.
	 * \param x The X coordinate relative to the screen of where to draw the objects.
	 * \param y The Y coordinate relative to the screen of where to draw the objects.
	 * \param flags The flags of how to draw the objects.
	 * \param palette The palette shift of which to draw the objects.
	 * \param affineSet The affine set to be used when drawing the objects.
	 * \param mtx The affine transformation matrix to use when drawing the object.
	 * \param unk8 
	 * \param settings The object processing settings.
	 * 
	 * \return True if the objects were pushed to the render list successfully, false otherwise.
	 * */
	bool drawMtx(GXOamAttr* oamAttrs, s32 x, s32 y, Flags flags, u8 palette, u8 affineSet, MtxFx22* mtx, s32 unk8, Settings settings);

	// OAM::drawMtxRot | 0x0200C7D4
	/**
	 * \brief Pushes an object to the top screen render list to be drawn. (Matrix w/rotation version)
	 * 
	 * \param oamAttrs The OAM object attribute array.
	 * \param x The X coordinate relative to the screen of where to draw the objects.
	 * \param y The Y coordinate relative to the screen of where to draw the objects.
	 * \param flags The flags of how to draw the objects.
	 * \param palette The palette shift of which to draw the objects.
	 * \param affineSet The affine set to be used when drawing the objects.
	 * \param mtx The affine transformation matrix to use when drawing the object.
	 * \param unk8 
	 * \param rot The rotation that the object will be drawn.
	 * \param rotCenter The rotation center.
	 * \param settings The object processing settings.
	 * 
	 * \return True if the objects were pushed to the render list successfully, false otherwise.
	 * */
	bool drawMtx(GXOamAttr* oamAttrs, s32 x, s32 y, Flags flags, u8 palette, u8 affineSet, MtxFx22* mtx, s32 unk8, fx16 rot, s16(*rotCenter)[2], Settings settings);
	
	// OAM::drawSprite | 0x0200D578
	/**
	 * \brief Pushes an object to the top screen render list to be drawn.
	 * 
	 * \param oamAttrs The OAM object attribute array.
	 * \param x The X coordinate relative to the level of where to draw the objects.
	 * \param y The Y coordinate relative to the level of where to draw the objects.
	 * \param flags The flags of how to draw the objects.
	 * \param palette The palette shift of which to draw the objects.
	 * \param affineSet The affine set to be used when drawing the objects.
	 * \param scale The scale that the objects will be drawn.
	 * \param rot The rotation that the object will be drawn.
	 * \param rotCenter The rotation center.
	 * \param settings The object processing settings.
	 * 
	 * \return True if the objects were pushed to the render list successfully, false otherwise.
	 * */
	bool drawSprite(GXOamAttr* oamAttrs, fx32 x, fx32 y, Flags flags, u8 palette, u8 affineSet, Vec2* scale, fx16 rot, s16(*rotCenter)[2], Settings settings);

	/* ======== BOTTOM SCREEN ======== */

	// OAM::drawSub | 0x0200C32C
	/**
	 * \brief Pushes an object to the bottom screen render list to be drawn.
	 * 
	 * \param oamAttrs The OAM object attribute array.
	 * \param x The X coordinate relative to the screen of where to draw the objects.
	 * \param y The Y coordinate relative to the screen of where to draw the objects.
	 * \param flags The flags of how to draw the objects.
	 * \param palette The palette shift of which to draw the objects.
	 * \param affineSet The affine set to be used when drawing the objects.
	 * \param settings The object processing settings.
	 * 
	 * \return True if the objects were pushed to the render list successfully, false otherwise.
	 * */
	bool drawSub(GXOamAttr* oamAttrs, s32 x, s32 y, Flags flags, u8 palette, u8 affineSet, Settings settings);

	// OAM::drawExtSub | 0x0200C600
	/**
	 * \brief Pushes an object to the bottom screen the render list to be drawn. (Extended version)
	 * 
	 * \param oamAttrs The OAM object attribute array.
	 * \param x The X coordinate relative to the screen of where to draw the objects.
	 * \param y The Y coordinate relative to the screen of where to draw the objects.
	 * \param flags The flags of how to draw the objects.
	 * \param palette The palette shift of which to draw the objects.
	 * \param affineSet The affine set to be used when drawing the objects.
	 * \param scale The scale that the objects will be drawn.
	 * \param rot The rotation that the object will be drawn.
	 * \param rotCenter The rotation center.
	 * \param settings The object processing settings.
	 * 
	 * \return True if the objects were pushed to the render list successfully, false otherwise.
	 * */
	bool drawSub(GXOamAttr* oamAttrs, s32 x, s32 y, Flags flags, u8 palette, u8 affineSet, Vec2* scale, fx16 rot, s16(*rotCenter)[2], Settings settings);

	// OAM::drawMtxSub | 0x0200BED4
	/**
	 * \brief Pushes an object to the bottom screen render list to be drawn. (Matrix version)
	 * 
	 * \param oamAttrs The OAM object attribute array.
	 * \param x The X coordinate relative to the screen of where to draw the objects.
	 * \param y The Y coordinate relative to the screen of where to draw the objects.
	 * \param flags The flags of how to draw the objects.
	 * \param palette The palette shift of which to draw the objects.
	 * \param affineSet The affine set to be used when drawing the objects.
	 * \param mtx The affine transformation matrix to use when drawing the object.
	 * \param settings The object processing settings.
	 * 
	 * \return True if the objects were pushed to the render list successfully, false otherwise.
	 * */
	bool drawMtxSub(GXOamAttr* oamAttrs, s32 x, s32 y, Flags flags, u8 palette, u8 affineSet, MtxFx22* mtx, Settings settings);

	// OAM::drawMtxRotSub | 0x0200B8C8
	/**
	 * \brief Pushes an object to the bottom screen render list to be drawn. (Matrix w/rotation version)
	 * 
	 * \param oamAttrs The OAM object attribute array.
	 * \param x The X coordinate relative to the screen of where to draw the objects.
	 * \param y The Y coordinate relative to the screen of where to draw the objects.
	 * \param flags The flags of how to draw the objects.
	 * \param palette The palette shift of which to draw the objects.
	 * \param affineSet The affine set to be used when drawing the objects.
	 * \param mtx The affine transformation matrix to use when drawing the object.
	 * \param rot The rotation that the object will be drawn.
	 * \param rotCenter The rotation center.
	 * \param settings The object processing settings.
	 * 
	 * \return True if the objects were pushed to the render list successfully, false otherwise.
	 * */
	bool drawMtxSub(GXOamAttr* oamAttrs, s32 x, s32 y, Flags flags, u8 palette, u8 affineSet, MtxFx22* mtx, fx16 rot, s16(*rotCenter)[2], Settings settings);

	/* ======== ANIMATION CLASS ======== */

	class Anim
	{
	public:

		struct Frame
		{
			GXOamAttr *oamAttrs;
			fx32 duration;
			u32 reserved;
		};

		struct FrameEntry
		{
			Frame* frames;
			u32 frameCount;
		};

		void init(FrameEntry* frameTable, u32 frameTableID, u8 flags, fx32 updateSpeed, u16 curFrameID);

		bool update();

		__attribute__((noinline))
		void setUpdateSpeed(fx32 updateSpeed)
		{
			if ( updateSpeed < 0 )
			{
				this->flags |= 4;
				this->updateSpeed = -updateSpeed;
			}
			else
			{
				this->flags &= ~4;
				this->updateSpeed = updateSpeed;
			}
		}

		__attribute__((noinline))
		bool draw(s32 x, s32 y, Flags flags, u8 palette, u8 affineSet, Vec2* scale, fx16 rot, s16(*rotCenter)[2], Settings settings) {
			return OAM::draw(this->frames[this->curFrameID].oamAttrs, x, y, flags, palette, affineSet, scale, rot, rotCenter, settings);
		}

		bool drawSprite(fx32 x, fx32 y, Flags flags, u8 palette, u8 affineSet, Vec2* scale, fx16 rot, s16(*rotCenter)[2], Settings settings);

		bool drawSub(s32 x, s32 y, Flags flags, u8 palette, u8 affineSet, Vec2* scale, fx16 rot, s16(*rotCenter)[2], Settings settings);
		
	private:

		Frame* frames; // The pointer to the frames.
		u32 frameTableID; // The ID of the current frame table.
		fx32 curFrameDur; // The duration of the current frame.
		fx32 updateSpeed; // The frame update speed.
		u16 curFrameID; // The ID of the current frame.
		u16 frameCount; // The frame count.
		u8 flags; // The animation flags.

	};

}

#endif //!NSMB_OAM_INCLUDED
