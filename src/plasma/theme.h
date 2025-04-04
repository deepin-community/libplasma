/*
    SPDX-FileCopyrightText: 2006-2007 Aaron Seigo <aseigo@kde.org>
    SPDX-FileCopyrightText: 2013 Marco Martin <mart@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef PLASMA_THEME_H
#define PLASMA_THEME_H

#include <QFont>
#include <QGuiApplication>
#include <QObject>

#include <KSharedConfig>
#include <plasma/plasma_export.h>

class KPluginMetaData;

namespace Plasma
{
class ThemePrivate;
class SvgPrivate;

/**
 * @class Theme plasma/theme.h <Plasma/Theme>
 *
 * @short Interface to the Plasma theme
 *
 *
 * Plasma::Theme provides access to a common and standardized set of graphic
 * elements stored in SVG format. This allows artists to create single packages
 * of SVGs that will affect the look and feel of all workspace components.
 *
 * Plasma::Svg uses Plasma::Theme internally to locate and load the appropriate
 * SVG data. Alternatively, Plasma::Theme can be used directly to retrieve
 * file system paths to SVGs by name.
 */
class PLASMA_EXPORT Theme : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString themeName READ themeName NOTIFY themeChanged)
    Q_PROPERTY(bool useGlobalSettings READ useGlobalSettings NOTIFY themeChanged)
    Q_PROPERTY(QString wallpaperPath READ wallpaperPath NOTIFY themeChanged)

    // fonts
    Q_PROPERTY(QFont defaultFont READ defaultFont NOTIFY defaultFontChanged)
    Q_PROPERTY(QFont smallestFont READ smallestFont NOTIFY smallestFontChanged)

    Q_PROPERTY(QPalette palette READ palette NOTIFY themeChanged)

public:
    enum ColorRole {
        TextColor = 0, /**<  the text color to be used by items resting on the background */
        BackgroundColor = 1, /**< the default background color */
        HighlightColor = 2, /**<  the text highlight color to be used by items resting
                                   on the background */
        HoverColor = 3, /**< color for hover effect on view */
        FocusColor = 4, /**< color for focus effect on view */
        LinkColor = 5, /**< color for clickable links */
        VisitedLinkColor = 6, /**< color visited clickable links */
        HighlightedTextColor = 7, /**< color contrasting with HighlightColor, to be used for instance with */
        PositiveTextColor = 8, /**< color of foreground objects with a "positive message" connotation (usually green) */
        NeutralTextColor = 9, /**< color of foreground objects with a "neutral message" connotation (usually yellow) */
        NegativeTextColor = 10, /**< color of foreground objects with a "negative message" connotation (usually red) */
        DisabledTextColor = 11, /**< color of disabled text @since 5.64 */
    };

    enum ColorGroup {
        NormalColorGroup = 0,
        ButtonColorGroup = 1,
        ViewColorGroup = 2,
        ComplementaryColorGroup = 3,
        HeaderColorGroup,
        ToolTipColorGroup,
    };
    Q_ENUM(ColorGroup)

    /**
     * Default constructor. It will be the global theme configured in plasmarc
     * @param parent the parent object
     */
    explicit Theme(QObject *parent = nullptr);

    /**
     * Construct a theme. It will be a custom theme instance of themeName.
     * @param themeName the name of the theme to create
     * @param parent the parent object
     * @since 4.3
     */
    explicit Theme(const QString &themeName, QObject *parent = nullptr);

    ~Theme() override;

    /**
     * Sets the current theme being used.
     */
    void setThemeName(const QString &themeName);

    /**
     * @return the name of the theme.
     */
    QString themeName() const;

    /**
     * Retrieve the path for an SVG image in the current theme.
     *
     * @param name the name of the file in the theme directory (without the
     *           ".svg" part or a leading slash)
     * @return the full path to the requested file for the current theme
     */
    QString imagePath(const QString &name) const;

    /**
     * Retrieves the default wallpaper associated with this theme.
     *
     * @param size the target height and width of the wallpaper; if an invalid size
     *           is passed in, then a default size will be provided instead.
     * @return the full path to the wallpaper image
     */
    QString wallpaperPath(const QSize &size = QSize()) const;

    Q_INVOKABLE QString wallpaperPathForSize(int width = -1, int height = -1) const;

    /**
     * Checks if this theme has an image named in a certain way
     *
     * @param name the name of the file in the theme directory (without the
     *           ".svg" part or a leading slash)
     * @return true if the image exists for this theme
     */
    bool currentThemeHasImage(const QString &name) const;

    /**
     * Returns the color scheme configurationthat goes along this theme.
     * This can be used with KStatefulBrush and KColorScheme to determine
     * the proper colours to use along with the visual elements in this theme.
     */
    KSharedConfigPtr colorScheme() const;

    /**
     * Returns the text color to be used by items resting on the background
     *
     * @param role which role (usage pattern) to get the color for
     * @param group which group we want a color of
     */
    QColor color(ColorRole role, ColorGroup group = NormalColorGroup) const;

    /**
     * Tells the theme whether to follow the global settings or use application
     * specific settings
     *
     * @param useGlobal pass in true to follow the global settings
     */
    void setUseGlobalSettings(bool useGlobal);

    /**
     * @return true if the global settings are followed, false if application
     * specific settings are used.
     */
    bool useGlobalSettings() const;

    /**
     * Returns a QPalette with the colors set as defined by the theme.
     * @since 5.68
     */
    QPalette palette() const;

    /**
     * @return plugin metadata for this theme, with information such as
     * name, description, author, website etc
     * @since 5.95
     */
    KPluginMetaData metadata() const;

    /**
     * @return The default application font
     * @since 5.0
     */
    QFont defaultFont() const;

    /**
     * @return The smallest readable font
     * @since 5.0
     */
    QFont smallestFont() const;

    /** This method allows Plasma to enable and disable the background
     * contrast effect for a given theme, improving readability. The
     * value is read from the "enabled" key in the "ContrastEffect"
     * group in the Theme's metadata file.
     * The configuration in the metadata.desktop file of the theme
     * could look like this (for a lighter background):
     * \code
     * [ContrastEffect]
     * enabled=true
     * contrast=0.45
     * intensity=0.45
     * saturation=1.7
     * \endcode
     * @return Whether or not to enable the contrasteffect
     * @since 5.0
     */
    bool backgroundContrastEnabled() const;

    /** This method allows Plasma to enable and disable the adaptive
     * transparency option of the panel, which allows user to decide
     * whether the panel should be always transparent, always opaque
     * or only opaque when a window is maximized.
     * The configuration in the metadata.desktop file of the theme
     * could look like this (for a lighter background):
     * \code
     * [AdaptiveTransparency]
     * enabled=true
     * \endcode
     * @return Whether or not to enable the adaptive transparency
     * @since 5.20
     */
    bool adaptiveTransparencyEnabled() const;

    /** This method allows Plasma to set a background contrast effect
     * for a given theme, improving readability. The value is read
     * from the "contrast" key in the "ContrastEffect" group in the
     * Theme's metadata file.
     * @return The contrast provided to the contrasteffect
     * @since 5.0
     * @see backgroundContrastEnabled
     */
    qreal backgroundContrast() const;

    /** This method allows Plasma to set a background contrast effect
     * for a given theme, improving readability. The value is read
     * from the "intensity" key in the "ContrastEffect" group in the
     * Theme's metadata file.
     * @return The intensity provided to the contrasteffect
     * @since 5.0
     * @see backgroundContrastEnabled
     */
    qreal backgroundIntensity() const;

    /** This method allows Plasma to set a background contrast effect
     * for a given theme, improving readability. The value is read
     * from the "saturation" key in the "ContrastEffect" group in the
     * Theme's metadata file.
     * @return The saturation provided to the contrasteffect
     * @since 5.0
     * @see backgroundContrastEnabled
     */
    qreal backgroundSaturation() const;

    /** This method allows Plasma to enable and disable the blurring
     * of what is behind the background for a given theme. The
     * value is read from the "enabled" key in the "BlurBehindEffect"
     * group in the Theme's metadata file. Default is @c true.
     *
     * The configuration in the metadata.desktop file of the theme
     * could look like this:
     * \code
     * [BlurBehindEffect]
     * enabled=false
     * \endcode
     * @return Whether or not to enable blurring of what is behind
     * @since 5.57
     */
    bool blurBehindEnabled() const;

    /**
     * Returns the size of the letter "M" as rendered on the screen with the given font.
     * This values gives you a base size that:
     * * scales dependent on the DPI of the screen
     * * Scales with the default font as set by the user
     * You can use it like this in QML Items:
     * \code
     * Item {
     *     width: PlasmaCore.Theme.mSize(PlasmaCore.Theme.defaultFont).height
     *     height: width
     * }
     * \endcode
     * This allows you to dynamically scale elements of your user interface with different font settings and
     * different physical outputs (with different DPI).
     * @param font The font to use for the metrics.
     * @return The size of the letter "M" as rendered on the screen with the given font.
     * @since 5.0
     */
    Q_INVOKABLE QSizeF mSize(const QFont &font = QGuiApplication::font()) const;

    QString backgroundPath(const QString &image) const;

    static QPalette globalPalette();

    static KSharedConfigPtr globalColorScheme();

Q_SIGNALS:
    /**
     * Emitted when the user changes the theme. Stylesheet usage, colors, etc. should
     * be updated at this point. However, SVGs should *not* be repainted in response
     * to this signal; connect to Svg::repaintNeeded() instead for that, as Svg objects
     * need repainting not only when themeChanged() is emitted; moreover Svg objects
     * connect to and respond appropriately to themeChanged() internally, emitting
     * Svg::repaintNeeded() at an appropriate time.
     */
    void themeChanged();

    /** Notifier for change of defaultFont property */
    void defaultFontChanged();
    /** Notifier for change of smallestFont property */
    void smallestFontChanged();

private:
    friend class SvgPrivate;
    friend class FrameSvg;
    friend class FrameSvgPrivate;
    friend class ThemePrivate;
    ThemePrivate *d;
};

} // Plasma namespace

#endif // multiple inclusion guard
