/*
    This source file is part of Konsole, a terminal emulator.

    Copyright 2006-2008 by Robert Knight <robertknight@gmail.com>

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

#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

// Qt
#include <QtGui/QFont>
#include <QtGui/QKeySequence>
#include <QtCore/QAbstractListModel>
#include <QtCore/QHash>
#include <QtCore/QList>
#include <QtCore/QSet>
#include <QtCore/QStringList>
#include <QtCore/QPair>
#include <QtCore/QPointer>
#include <QtCore/QVariant>
#include <QtCore/QStack>

// Konsole
#include "Profile.h"

class QSignalMapper;


namespace Konsole
{

class Session;

/**
 * Manages running terminal sessions and the profiles which specify various
 * settings for terminal sessions and their displays.
 *
 * Profiles in the manager have a concept of favorite status, which can be used
 * by widgets and dialogs in the application decide which sessions to list and
 * how to display them.  The favorite status of a profile can be altered using
 * setFavorite() and retrieved using isFavorite()
 */
class KONSOLEPRIVATE_EXPORT SessionManager : public QObject
{
    Q_OBJECT

public:
    /**
     * Constructs a new session manager and loads information about the available
     * profiles.
     */
    SessionManager();
    void setMenuOrder();

    /**
     * Destroys the SessionManager. All running sessions should be closed
     * (via closeAll()) before the SessionManager is destroyed.
     */
    virtual ~SessionManager();

    /** Kill all running sessions. */
    void closeAll();

    /**
     * Returns a list of profiles which have been loaded.
     * Initially only the profile currently set as the default is loaded.
     *
     * Favorite profiles are loaded automatically when findFavorites() is called.
     *
     * All other profiles can be loaded by calling loadAllProfiles().  This may
     * involves opening, reading and parsing all profiles from disk, and
     * should only be done when necessary.
     */
    QList<Profile::Ptr> loadedProfiles() const;

    QList<Profile::Ptr> sortedFavorites();

    /*
     * Sorts the profile list by menuindex; those without an menuindex, sort by name.
     *  The menuindex list is first and then the non-menuindex list.
     *
     * @param list The profile list to sort
     */
    void sortProfiles(QList<Profile::Ptr>& list);

    /**
     * Searches for available profiles on-disk and returns a list
     * of paths of profiles which can be loaded.
     */
    QStringList availableProfilePaths() const;


    /**
     * Loads a profile from the specified path and registers
     * it with the SessionManager.
     *
     * @p path may be relative or absolute.  The path may just be the
     * base name of the profile to load (eg. if the profile's full path
     * is "<konsole data dir>/My Profile.profile" then both
     * "konsole/My Profile.profile" , "My Profile.profile" and
     * "My Profile" will be accepted)
     *
     * @return Pointer to a profile which can be passed to createSession()
     * to create a new session using this profile.
     */
    Profile::Ptr loadProfile(const QString& path);

    /**
     * Updates a @p profile with the changes specified in @p propertyMap.
     *
     * All sessions currently using the profile will be updated to reflect the new settings.
     *
     * After the profile is updated, the profileChanged() signal will be emitted.
     *
     * @param profile The profile to change
     * @param propertyMap A map between profile properties and values describing the changes
     * @param persistent If true, the changes are saved to the profile's configuration file,
     * set this to false if you want to preview possible changes to a profile but do not
     * wish to make them permanent.
     */
    void changeProfile(Profile::Ptr profile , QHash<Profile::Property, QVariant> propertyMap,
                       bool persistent = true);

    /**
     * Returns a Profile object describing the default type of session, which is used
     * if createSession() is called with an empty configPath argument.
     */
    Profile::Ptr defaultProfile() const;
    /**
     * Returns a Profile object with hard-coded settings which is always available.
     * This can be used as a parent for new profiles which provides suitable default settings
     * for all properties.
     */
    Profile::Ptr fallbackProfile() const;

    /**
     * Creates a new session using the settings specified by the specified
     * profile.
     *
     * The new session has no views associated with it.  A new TerminalDisplay view
     * must be created in order to display the output from the terminal session and
     * send keyboard or mouse input to it.
     *
     * @param profile A profile containing the settings for the new session.  If @p profile
     * is null the default profile (see defaultProfile()) will be used.
     */
    Session* createSession(Profile::Ptr profile = Profile::Ptr());

    /** Returns the profile associated with a session. */
    Profile::Ptr sessionProfile(Session* session) const;
    /** Sets the profile associated with a session. */
    void setSessionProfile(Session* session, Profile::Ptr profile);

    /**
     * Updates a session's properties to match its current profile.
     */
    void updateSession(Session* session);

    /**
     * Returns a list of active sessions.
     */
    const QList<Session*> sessions() const;

    /**
     * Deletes the configuration file used to store a profile.
     * The profile will continue to exist while sessions are still using it.  The profile
     * will be marked as hidden (see Profile::setHidden() ) so that it does not show
     * up in profile lists and future changes to the profile are not stored to disk.
     *
     * Returns true if the profile was successfully deleted or false otherwise.
     */
    bool deleteProfile(Profile::Ptr profile);

    /**
     * Sets the @p profile as the default profile for new sessions created
     * with createSession()
     */
    void setDefaultProfile(Profile::Ptr profile);

    /**
     * Returns the set of the user's favorite profiles.
     */
    QSet<Profile::Ptr> findFavorites();

    /**
     * Returns the list of shortcut key sequences which
     * can be used to create new sessions based on
     * existing profiles
     *
     * When one of the shortcuts is activated,
     * use findByShortcut() to load the profile associated
     * with the shortcut.
     */
    QList<QKeySequence> shortcuts();

    /**
     * Finds and loads the profile associated with
     * the specified @p shortcut key sequence and returns a pointer to it.
     */
    Profile::Ptr findByShortcut(const QKeySequence& shortcut);

    /**
     * Associates a shortcut with a particular profile.
     */
    void setShortcut(Profile::Ptr profile , const QKeySequence& shortcut);

    /** Returns the shortcut associated with a particular profile. */
    QKeySequence shortcut(Profile::Ptr profile) const;

    /**
     * Registers a new type of session.
     * The favorite status of the session ( as returned by isFavorite() ) is set to false by default.
     */
    void addProfile(Profile::Ptr type);

    /**
     * Specifies whether a profile should be included in the user's
     * list of favorite sessions.
     */
    void setFavorite(Profile::Ptr profile , bool favorite);

    /**
     * Loads all available profiles.  This involves reading each
     * profile configuration file from disk and parsing it.
     * Therefore it should only be done when necessary.
     */
    void loadAllProfiles();

    /**
     * Sets the global session manager instance.
     */
    static void setInstance(SessionManager* instance);
    /**
     * Returns the session manager instance.
     */
    static SessionManager* instance();

    // session management
    void saveSessions(KConfig* config);
    int  getRestoreId(Session* session);
    void restoreSessions(KConfig* config);
    Session* idToSession(int id);

signals:
    /** Emitted when a profile is added to the manager. */
    void profileAdded(Profile::Ptr ptr);
    /** Emitted when a profile is removed from the manager. */
    void profileRemoved(Profile::Ptr ptr);
    /** Emitted when a profile's properties are modified. */
    void profileChanged(Profile::Ptr ptr);

    /**
     * Emitted when a session's settings are updated to match
     * its current profile.
     */
    void sessionUpdated(Session* session);

    /**
     * Emitted when the favorite status of a profile changes.
     *
     * @param profile The profile to change
     * @param favorite Specifies whether the session is a favorite or not
     */
    void favoriteStatusChanged(Profile::Ptr profile , bool favorite);

    /**
     * Emitted when the shortcut for a profile is changed.
     *
     * @param profile The profile whose status was changed
     * @param newShortcut The new shortcut key sequence for the profile
     */
    void shortcutChanged(Profile::Ptr profile , const QKeySequence& newShortcut);

public slots:

    /** Saves settings (favorites, shortcuts, default profile etc.) to disk. */
    void saveSettings();

protected slots:

    /**
     * Called to inform the manager that a session has finished executing.
     *
     * @param session The Session which has finished executing.
     */
    void sessionTerminated(QObject* session);

private slots:
    void sessionProfileCommandReceived(const QString& text);

private:


    // loads the mappings between shortcut key sequences and
    // profile paths
    void loadShortcuts();
    // saves the mappings between shortcut key sequences and
    // profile paths
    void saveShortcuts();

    //loads the set of favorite sessions
    void loadFavorites();
    //saves the set of favorite sessions
    void saveFavorites();
    // saves a profile to a file
    // returns the path to which the profile was saved, which will
    // be the same as the path property of profile if valid or a newly generated path
    // otherwise
    QString saveProfile(Profile::Ptr profile);

    // applies updates to a profile
    // to all sessions currently using that profile
    // if modifiedPropertiesOnly is true, only properties which
    // are set in the profile @p key are updated
    void applyProfile(Profile::Ptr profile , bool modifiedPropertiesOnly);
    // apples updates to the profile @p profile to the session @p session
    // if modifiedPropertiesOnly is true, only properties which
    // are set in @p profile are update ( ie. properties for which profile->isPropertySet(<property>)
    // returns true )
    void applyProfile(Session* session , const Profile::Ptr profile , bool modifiedPropertiesOnly);

    QSet<Profile::Ptr> _profiles;
    QHash<Session*, Profile::Ptr> _sessionProfiles;
    QHash<Session*, Profile::Ptr> _sessionRuntimeProfiles;
    QHash<Session*, int> _restoreMapping;

    struct ShortcutData {
        Profile::Ptr profileKey;
        QString profilePath;
    };
    QMap<QKeySequence, ShortcutData> _shortcuts; // shortcut keys -> profile path

    QList<Session*> _sessions; // list of running sessions

    Profile::Ptr _defaultProfile;
    Profile::Ptr _fallbackProfile;

    QSet<Profile::Ptr> _favorites; // list of favorite profiles

    bool _loadedAllProfiles; // set to true after loadAllProfiles has been called
    bool _loadedFavorites; // set to true after loadFavorites has been called
    QSignalMapper* _sessionMapper;
};

/** Utility class to simplify code in SessionManager::applyProfile(). */
class ShouldApplyProperty
{
public:
    ShouldApplyProperty(const Profile::Ptr profile , bool modifiedOnly) :
        _profile(profile) , _modifiedPropertiesOnly(modifiedOnly) {}

    bool shouldApply(Profile::Property property) const {
        return !_modifiedPropertiesOnly || _profile->isPropertySet(property);
    }
private:
    const Profile::Ptr _profile;
    bool _modifiedPropertiesOnly;
};

/**
 * PopStackOnExit is a utility to remove all values from a QStack which are added during
 * the lifetime of a PopStackOnExit instance.
 *
 * When a PopStackOnExit instance is destroyed, elements are removed from the stack
 * until the stack count is reduced the value when the PopStackOnExit instance was created.
 */
template <class T>
class PopStackOnExit
{
public:
    PopStackOnExit(QStack<T>& stack) : _stack(stack) , _count(stack.count()) {}
    ~PopStackOnExit() {
        while (_stack.count() > _count)
            _stack.pop();
    }
private:
    QStack<T>& _stack;
    int _count;
};
/**
 * Item-view model which contains a flat list of sessions.
 * After constructing the model, call setSessions() to set the sessions displayed
 * in the list.  When a session ends (after emitting the finished() signal) it is
 * automatically removed from the list.
 *
 * The internal pointer for each item in the model (index.internalPointer()) is the
 * associated Session*
 */
class SessionListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    SessionListModel(QObject* parent = 0);

    /**
     * Sets the list of sessions displayed in the model.
     * To display all sessions that are currently running in the list,
     * call setSessions(SessionManager::instance()->sessions())
     */
    void setSessions(const QList<Session*>& sessions);

    // reimplemented from QAbstractItemModel
    virtual QModelIndex index(int row, int column, const QModelIndex& parent) const;
    virtual QVariant data(const QModelIndex& index, int role) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation,
                                int role) const;
    virtual int columnCount(const QModelIndex& parent) const;
    virtual int rowCount(const QModelIndex& parent) const;
    virtual QModelIndex parent(const QModelIndex& index) const;

protected:
    virtual void sessionRemoved(Session*) {}

private slots:
    void sessionFinished();

private:
    QList<Session*> _sessions;
};

}
#endif //SESSIONMANAGER_H
