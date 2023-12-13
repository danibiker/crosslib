// Written by Michel Helms (MichelHelms@Web.de).
// Parts of code were taken from Idael Cardoso (http://www.codeproject.com/csharp/csharpripper.asp)
//   and from Larry Osterman (http://blogs.msdn.com/larryosterman/archive/2005/05.aspx).
// Finished at 26th of September in 2006
// Of course you are allowed to cut this lines off ;)




#include "CAudioCD.h"
#include "AudioCD_Helpers.h"

// Constructor / Destructor
CAudioCD::CAudioCD( char Drive )
{
	m_hCD = NULLHANDLE;
	if ( Drive != '\0' )
		Open( Drive );
    cddbDiscid = "";
    m_dwTotalSecs=0;
}


CAudioCD::~CAudioCD()
{
	Close();
}




// Open / Close access
BOOL CAudioCD::Open( char Drive )
{
	Close();
#ifdef WIN
	// Open drive-handle
	char Fn[8] = { '\\', '\\', '.', '\\', Drive, ':', '\0' };
	if ( INVALID_HANDLE_VALUE == ( m_hCD = CreateFile( Fn, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL ) ) )
	{
		m_hCD = NULL;
		return FALSE;
	}

	// Lock drive
	if ( ! LockCD() )
	{
		UnlockCD();
		CloseHandle( m_hCD );
		m_hCD = NULL;
		return FALSE;
	}

	// Get track-table and add it to the intern array
	ULONG BytesRead;
	CDROM_TOC Table;
	if ( 0 == DeviceIoControl( m_hCD, IOCTL_CDROM_READ_TOC, NULL, 0, &Table, sizeof(Table), &BytesRead, NULL )){
		UnlockCD();
		CloseHandle( m_hCD );
		m_hCD = NULL;
		return FALSE;
	}

	for ( ULONG i=Table.FirstTrack-1; i<Table.LastTrack; i++ ){
		CDTRACK NewTrack;
		NewTrack.Address = AddressToSectors( Table.TrackData[i].Address );
		NewTrack.Length = AddressToSectors( Table.TrackData[i+1].Address ) - NewTrack.Address;
		NewTrack.Offset = AddressToSectors(Table.TrackData[i].Address);
		m_aTracks.push_back( NewTrack );
	}

    char buffer [50];
    memset(buffer, 0, 50);
    //sprintf (buffer, "%08x", cddb_discid());
    sprintf (buffer, "%08lu", getDiscID());
    cddbDiscid = std::string(buffer);
#endif
	// Return if track-count > 0
	return m_aTracks.size() > 0;
}


BOOL CAudioCD::IsOpened()
{
	return m_hCD != NULLHANDLE;
}


void CAudioCD::Close()
{
	UnlockCD();
	m_aTracks.clear();
#ifdef WIN
	CloseHandle( m_hCD );
#endif
	m_hCD = NULLHANDLE;
}




// Read / Get track-data
ULONG CAudioCD::GetTrackCount()
{
	if ( m_hCD == NULLHANDLE )
		return 0xFFFFFFFF;
	return m_aTracks.size();
}


ULONG CAudioCD::GetTrackTime( ULONG Track )
{
	if ( m_hCD == NULLHANDLE )
		return 0xFFFFFFFF;
	if ( Track >= m_aTracks.size() )
		return 0xFFFFFFFF;

	CDTRACK& Tr = m_aTracks.at(Track);
	return Tr.Length / 75;
}


ULONG CAudioCD::GetTrackSize( ULONG Track )
{
	if ( m_hCD == NULLHANDLE )
		return 0xFFFFFFFF;
	if ( Track >= m_aTracks.size() )
		return 0xFFFFFFFF;

	CDTRACK& Tr = m_aTracks.at(Track);
	return Tr.Length * RAW_SECTOR_SIZE;
}


BOOL CAudioCD::ReadTrack( ULONG TrackNr, CBuf<char>* pBuf )
{
	if ( m_hCD == NULLHANDLE )
		return FALSE;

	if ( TrackNr >= m_aTracks.size() )
		return FALSE;

#ifdef WIN
        CDTRACK& Track = m_aTracks.at(TrackNr);

	pBuf->Alloc( Track.Length*RAW_SECTOR_SIZE );

	RAW_READ_INFO Info;
	Info.TrackMode = CDDA;
	Info.SectorCount = SECTORS_AT_READ;

	for ( ULONG i=0; i<Track.Length/SECTORS_AT_READ; i++ )
	{
		Info.DiskOffset.QuadPart = (Track.Address + i*SECTORS_AT_READ) * CD_SECTOR_SIZE;
		ULONG Dummy;
		if ( 0 == DeviceIoControl( m_hCD, IOCTL_CDROM_RAW_READ, &Info, sizeof(Info), pBuf->Ptr()+i*SECTORS_AT_READ*RAW_SECTOR_SIZE, SECTORS_AT_READ*RAW_SECTOR_SIZE, &Dummy, NULL ) )
		{
			pBuf->Free();
			return FALSE;
		}
	}

        ULONG i = Track.Length/SECTORS_AT_READ;
	Info.SectorCount = Track.Length % SECTORS_AT_READ;
	Info.DiskOffset.QuadPart = (Track.Address + i*SECTORS_AT_READ) * CD_SECTOR_SIZE;
	ULONG Dummy;
	if ( 0 == DeviceIoControl( m_hCD, IOCTL_CDROM_RAW_READ, &Info, sizeof(Info), pBuf->Ptr()+i*SECTORS_AT_READ*RAW_SECTOR_SIZE, SECTORS_AT_READ*RAW_SECTOR_SIZE, &Dummy, NULL ) )
	{
		pBuf->Free();
		return FALSE;
	}
#endif
	return TRUE;
}


int CAudioCD::ExtractTrack( ULONG TrackNr,const char *Path )
{
	if ( m_hCD == NULLHANDLE )
		return 1;

	ULONG Dummy;

	if ( TrackNr >= m_aTracks.size() )
		return 2;
#ifdef WIN
	CDTRACK& Track = m_aTracks.at(TrackNr);

	HANDLE hFile = CreateFile( Path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
	if ( hFile == INVALID_HANDLE_VALUE )
		return 3;

	CWaveFileHeader WaveFileHeader( 44100, 16, 2, Track.Length*RAW_SECTOR_SIZE );
	WriteFile( hFile, &WaveFileHeader, sizeof(WaveFileHeader), &Dummy, NULL );

	CBuf<char> Buf( SECTORS_AT_READ * RAW_SECTOR_SIZE );

	RAW_READ_INFO Info;
	Info.TrackMode = CDDA;
	Info.SectorCount = SECTORS_AT_READ;

	for ( ULONG i=0; i<Track.Length/SECTORS_AT_READ; i++ )
	{
		Info.DiskOffset.QuadPart = (Track.Address + i*SECTORS_AT_READ) * CD_SECTOR_SIZE;
		if ( 0 == DeviceIoControl( m_hCD, IOCTL_CDROM_RAW_READ, &Info, sizeof(Info), Buf, SECTORS_AT_READ*RAW_SECTOR_SIZE, &Dummy, NULL ) )
			return 4;

		WriteFile( hFile, Buf, Buf.Size(), &Dummy, NULL );
	}

    ULONG i = Track.Length/SECTORS_AT_READ;
	Info.SectorCount = Track.Length % SECTORS_AT_READ;
	Info.DiskOffset.QuadPart = (Track.Address + i*SECTORS_AT_READ) * CD_SECTOR_SIZE;
	if ( 0 == DeviceIoControl( m_hCD, IOCTL_CDROM_RAW_READ, &Info, sizeof(Info), Buf, Info.SectorCount*RAW_SECTOR_SIZE, &Dummy, NULL ) )
		return CloseHandle( hFile ) == true ? 0 : 5;

	WriteFile( hFile, Buf, Info.SectorCount*RAW_SECTOR_SIZE, &Dummy, NULL );
        //Traza::print("Cerrando fichero extraido: " + string(Path), W_DEBUG);
	return CloseHandle( hFile ) == true ? 0 : 6;
#else
        return 1;
#endif
}




// Lock / Unlock CD-Rom Drive
BOOL CAudioCD::LockCD()
{
	if ( m_hCD == NULLHANDLE )
		return FALSE;
#ifdef WIN
        ULONG Dummy;
	PREVENT_MEDIA_REMOVAL pmr = { TRUE };
	return 0 != DeviceIoControl( m_hCD, IOCTL_STORAGE_MEDIA_REMOVAL, &pmr, sizeof(pmr), NULL, 0, &Dummy, NULL );
#else
        return FALSE;
#endif
}


BOOL CAudioCD::UnlockCD()
{
	if ( m_hCD == NULLHANDLE )
		return FALSE;
#ifdef WIN
	ULONG Dummy;
	PREVENT_MEDIA_REMOVAL pmr = { FALSE };
	return 0 != DeviceIoControl( m_hCD, IOCTL_STORAGE_MEDIA_REMOVAL, &pmr, sizeof(pmr), NULL, 0, &Dummy, NULL );
#else
        return 0;
#endif
}




// General operations
BOOL CAudioCD::InjectCD()
{
	if ( m_hCD == NULLHANDLE )
		return FALSE;
#ifdef WIN
	ULONG Dummy;
	return 0 != DeviceIoControl( m_hCD, IOCTL_STORAGE_LOAD_MEDIA, NULL, 0, NULL, 0, &Dummy, NULL );
#else
        return 0;
#endif
}


BOOL CAudioCD::IsCDReady( char Drive )
{
#ifdef WIN
        HANDLE hDrive;
	if ( Drive != '\0' )
	{
		// Open drive-handle if a drive is specified
		char Fn[8] = { '\\', '\\', '.', '\\', Drive, ':', '\0' };
		if ( INVALID_HANDLE_VALUE == ( hDrive = CreateFile( Fn, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL ) ) )
			return FALSE;
	}
	else
	{
		// Otherwise, take our open handle
		if ( m_hCD == NULL )
			return FALSE;
		hDrive = m_hCD;
	}

	ULONG Dummy;
	BOOL Success = DeviceIoControl( hDrive, IOCTL_STORAGE_CHECK_VERIFY2, NULL, 0, NULL, 0, &Dummy, NULL );

	if ( m_hCD != hDrive )
		CloseHandle( hDrive );

	return Success;
#else
        return 0;
#endif
}


BOOL CAudioCD::EjectCD()
{
	if ( m_hCD == NULLHANDLE )
		return FALSE;
	ULONG Dummy;
#ifdef WIN
	return 0 != DeviceIoControl( m_hCD, IOCTL_STORAGE_EJECT_MEDIA, NULL, 0, NULL, 0, &Dummy, NULL );
#else
        return 0;
#endif
}

/**
*
*/
int CAudioCD::cddb_sum(int n){
	int	ret;

	/* For backward compatibility this algorithm must not change */

	ret = 0;

	while (n > 0) {
		ret = ret + (n % 10);
		n = n / 10;
	}

	return (ret);
}


/**
* http://www.robots.ox.ac.uk/~spline/cddb-howto.txt
*/
DWORD CAudioCD::getDiscID()
{
	DWORD	dwRet;
	DWORD	t = 0;
	DWORD	n = 0;
	DWORD	dwTwoSecOffset;
	DWORD TRACKSPERSEC = 75;
	int m_nNumTracks = m_aTracks.size();
    DWORD dwSectorsNext;
    DWORD dwSectors;
	// Clear total number of sectors
	m_dwTotalSecs=0;

	// For backward compatibility this algorithm, do not change
	dwTwoSecOffset=2*TRACKSPERSEC;

	// Loop through all the tracks, excluding the Lead Out track
	for (int i = 0; i < m_nNumTracks; i++)
	{
		// Keep in mind the two seconds offset
		dwSectors = m_aTracks.at(i).Offset+dwTwoSecOffset;
		n += cddb_sum(dwSectors/TRACKSPERSEC);
        // Keep in mind the two seconds offset
        if (i + 1 < m_nNumTracks){
            dwSectorsNext = m_aTracks.at(i+1).Offset+dwTwoSecOffset;
        } else {
            dwSectorsNext = m_aTracks.at(i).Offset + m_aTracks.at(i).Length + dwTwoSecOffset;
        }
		t += (dwSectorsNext/TRACKSPERSEC-dwSectors/TRACKSPERSEC);
	}

	dwRet=( (n % 0xff) << 24 | t << 8 | (DWORD)(m_nNumTracks));

    DWORD leadout = (m_aTracks.at(m_nNumTracks - 1).Offset + m_aTracks.at(m_nNumTracks - 1).Length);
	// Get total playing time
	m_dwTotalSecs=(leadout+dwTwoSecOffset)/TRACKSPERSEC;

	return dwRet;
}
