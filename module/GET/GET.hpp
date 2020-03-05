/*
** EPITECH PROJECT, 2020
** CPP_zia_2019
** File description:
** GET
*/

#pragma once

#include <map>
#include <openZia/IModule.hpp>
#include <openZia/Pipeline.hpp>

namespace Zia {
	namespace Module {
		class GET : public oZ::IModule {
		public:
			GET() = default;
			virtual ~GET() = default;

			virtual const char *getName(void) const;
			virtual void onRegisterCallbacks(oZ::Pipeline &pipeline);

		protected:
		private:
			bool onInterpret(oZ::Context &context);
			bool _set404(oZ::Context &context) const noexcept;
			bool _setRegularFile(oZ::Context &context, const std::string &path) const noexcept;
			bool _setDirectory(oZ::Context &context, const std::string &path) const noexcept;
			bool _setError(oZ::Context &context) const noexcept;
		};
	};
};

static std::map<std::string, std::string> ContentTypes = {
	{".TXT", "text/plain"},
	{".HTM", "text/html"},
	{".TIFF", "image/tiff"},
	{".TMP", "application/octet-stream"},
	{".TOAST", "application/octet-stream"},
	{".TORRENT", "application/x-bittorrent"},
	{".TTF", "application/x-font-ttf"},
	{".UUE", "application/octet-stream"},
	{".VCD", "application/x-cdlink"},
	{".VCF", "text/x-vcard"},
	{".VCXPROJ", "application/xml"},
	{".VOB", "application/octet-stream"},
	{".WAV", "audio/x-wav"},
	{".WMA", "audio/x-ms-wma"},
	{".WMV", "video/x-ms-wmv"},
	{".WPD", "application/wordperfect"},
	{".WPS", "application/octet-stream"},
	{".XCODEPROJ", "application/octet-stream"},
	{".XHTML", "application/xhtml+xml"},
	{".XLR", "application/octet-stream"},
	{".XLS", "application/vnd.ms-excel"},
	{".XLSX", "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"},
	{".XML", "text/xml"},
	{".YUV", "application/octet-stream"},
	{".ZIPX", "application/octet-stream"},
	{".3DM", "application/octet-stream"},
	{".3DS", "application/octet-stream"},
	{".3G2", "video/3gpp2"},
	{".3GP", "video/3gpp"},
	{".ACCDB", "application/octet-stream"},
	{".AI", "application/illustrator"},
	{".AIF", "audio/x-aiff"},
	{".APK", "application/vnd.android.package-archive"},
	{".APP", "application/octet-stream"},
	{".ASF", "video/x-ms-asf"},
	{".ASP", "application/octet-stream"},
	{".ASPX", "application/xml"},
	{".AVI", "video/x-msvideo"},
	{".BAK", "application/octet-stream"},
	{".BIN", "application/octet-stream"},
	{".BMP", "image/bmp"},
	{".C", "text/x-csrc"},
	{".CAB", "application/octet-stream"},
	{".CBR", "application/octet-stream"},
	{".CER", "application/x-x509-ca-cert"},
	{".CFG", "application/octet-stream"},
	{".CFM", "application/octet-stream"},
	{".CGI", "application/octet-stream"},
	{".CLASS", "application/octet-stream"},
	{".CPP", "text/x-c++src"},
	{".CRDOWNLOAD", "application/octet-stream"},
	{".CRX", "application/x-chrome-extension"},
	{".CS", "text/plain"},
	{".CSR", "application/octet-stream"},
	{".CSS", "text/css"},
	{".CSV", "text/csv"},
	{".CUE", "application/octet-stream"},
	{".CUR", "application/octet-stream"},
	{".DAT", "application/octet-stream"},
	{".DB", "application/octet-stream"},
	{".DBF", "application/octet-stream"},
	{".DDS", "image/vnd.ms-dds"},
	{".DEB", "application/x-debian-package"},
	{".DEM", "application/octet-stream"},
	{".DESKTHEMEPACK", "application/octet-stream"},
	{".DLL", "application/octet-stream"},
	{".DMG", "application/octet-stream"},
	{".DMP", "application/octet-stream"},
	{".DOC", "application/msword"},
	{".DOCX", "application/vnd.openxmlformats-officedocument.wordprocessingml.document"},
	{".DRV", "application/octet-stream"},
	{".DTD", "application/xml-dtd"},
	{".DWG", "application/octet-stream"},
	{".DXF", "application/dxf"},
	{".EPS", "application/postscript"},
	{".FLA", "application/octet-stream"},
	{".FLV", "video/x-flv"},
	{".FNT", "application/octet-stream+fnt"},
	{".FON", "application/octet-stream+fon"},
	{".GADGET", "application/octet-stream"},
	{".GAM", "application/octet-stream"},
	{".GED", "application/octet-stream"},
	{".GIF", "image/gif"},
	{".GPX", "application/gpx+xml"},
	{".GZ", "application/x-gzip"},
	{".H", "text/x-chdr"},
	{".HQX", "application/mac-binhex40"},
	{".HTML", "text/html"},
	{".ICNS", "application/octet-stream"},
	{".ICO", "image/x-icon"},
	{".ICS", "text/calendar"},
	{".IFF", "application/octet-stream"},
	{".INDD", "application/octet-stream"},
	{".INI", "application/octet-stream"},
	{".ISO", "application/octet-stream"},
	{".JAVA", "application/octet-stream"},
	{".JPG", "image/jpeg"},
	{".JPEG", "image/jpeg"},
	{".JSP", "application/octet-stream"},
	{".JSON", "application/json"},
	{".KEY", "application/octet-stream"},
	{".KEYCHAIN", "application/octet-stream"},
	{".KML", "application/vnd.google-earth.kml+xml"},
	{".KMZ", "application/vnd.google-earth.kmz"},
	{".LOG", "application/octet-stream"},
	{".LUA", "application/octet-stream"},
	{".M", "application/octet-stream"},
	{".M3U", "audio/x-mpegurl"},
	{".M4A", "audio/mp4"},
	{".M4V", "video/x-m4v"},
	{".MAX", "application/octet-stream"},
	{".MDB", "application/octet-stream"},
	{".MDF", "application/octet-stream"},
	{".MID", "audio/midi"},
	{".MIM", "application/octet-stream"},
	{".MOV", "video/quicktime"},
	{".MP3", "audio/mpeg"},
	{".MP4", "video/mp4"},
	{".MPA", "audio/mpeg"},
	{".MPG", "video/mpeg"},
	{".MSG", "application/octet-stream"},
	{".NES", "application/octet-stream"},
	{".OBJ", "application/octet-stream"},
	{".ODT", "application/vnd.oasis.opendocument.text"},
	{".OTF", "application/vnd.oasis.opendocument.formula-template"},
	{".PAGES", "application/x-iwork-pages-sffpages"},
	{".PART", "application/octet-stream"},
	{".PCT", "application/octet-stream"},
	{".PDB", "chemical/x-pdb"},
	{".PDF", "application/pdf"},
	{".PHP", "application/x-httpd-php"},
	{".PKG", "application/octet-stream"},
	{".PL", "application/octet-stream"},
	{".PLUGIN", "application/octet-stream"},
	{".PNG", "image/png"},
	{".PPS", "application/vnd.ms-powerpoint"},
	{".PPT", "application/vnd.ms-powerpoint"},
	{".PPTX", "application/vnd.openxmlformats-officedocument.presentationml.presentation"},
	{".PRF", "application/pics-rules"},
	{".PS", "application/postscript"},
	{".PSD", "application/photoshop"},
	{".PSPIMAGE", "application/octet-stream"},
	{".PY", "application/octet-stream"},
	{".RM", "audio/x-pn-realaudio"},
	{".ROM", "application/octet-stream"},
	{".RPM", "application/x-rpm"},
	{".RSS", "application/octet-stream"},
	{".RTF", "application/rtf"},
	{".SAV", "application/octet-stream"},
	{".SDF", "application/octet-stream"},
	{".SH", "application/x-sh"},
	{".SITX", "application/octet-stream"},
	{".SLN", "text/plain"},
	{".SQL", "application/octet-stream"},
	{".SRT", "application/octet-stream"},
	{".SVG", "image/svg+xml"},
	{".SWF", "application/x-shockwave-flash"},
	{".SWIFT", "application/octet-stream"},
	{".TAX2016", "application/octet-stream"},
	{".TEX", "application/x-tex"}};