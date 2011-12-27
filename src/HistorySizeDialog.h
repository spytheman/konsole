/*
    Copyright 2007-2008 by Robert Knight <robertknight@gmail.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
    02110-1301  USA.
*/

#ifndef HISTORYSIZEDIALOG_H
#define HISTORYSIZEDIALOG_H

// KDE
#include <KDialog>

class QAbstractButton;
class KIntSpinBox;

namespace Konsole
{

/**
 * A dialog which allows the user to select the number of lines of output
 * which are remembered for a session.
 */
class HistorySizeDialog : public KDialog
{
    Q_OBJECT

public:
    /**
     * Construct a new history size dialog.
     */
    HistorySizeDialog(QWidget* parent);

    /** Specifies the type of history scroll */
    enum HistoryMode {
        /**
         * No history.  Lines of output are lost
         * as soon as they are scrolled off-screen.
         */
        NoHistory,
        /**
         * A history which stores up to a fixed number of lines
         * in memory.
         */
        FixedSizeHistory,
        /**
         * An 'unlimited' history which stores lines of output in
         * a file on disk.
         */
        UnlimitedHistory
    };


    /** Specifies the history mode. */
    void setMode(HistoryMode mode);
    /** Returns the history mode chosen by the user. */
    HistoryMode mode() const;
    /**
     * Returns the number of lines of history to remember.
     * This is only valid when mode() == FixedSizeHistory,
     * and returns 0 otherwise.
     */
    int lineCount() const;
    /** Sets the number of lines for the fixed size history mode. */
    void setLineCount(int lines);

signals:
    /**
     * Emitted when the user changes the scroll-back mode or line count and
     * accepts the change by pressing the OK button
     *
     * @param mode The current history mode.  This is a value from the HistoryMode enum.
     * @param lineCount The current line count.  This is only applicable if mode is
     * FixedSizeHistory
     */
    void optionsChanged(int mode , int lineCount);

private slots:

    // fires the optionsChanged() signal with the current mode
    // and line count as arguments
    void emitOptionsChanged();

private:
    QAbstractButton* _noHistoryButton;
    QAbstractButton* _fixedHistoryButton;
    QAbstractButton* _unlimitedHistoryButton;
    KIntSpinBox* _lineCountBox;

    // 1000 lines was the default in the KDE 3 series
    static const int defaultLineCount = 1000;
};

}

#endif // HISTORYSIZEDIALOG_H
