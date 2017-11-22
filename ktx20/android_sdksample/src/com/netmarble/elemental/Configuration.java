
package com.netmarble.elemental;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.text.TextUtils;
import android.util.Log;
import android.widget.EditText;

public class Configuration
{
	private final static String[] ZONES              = { "dev", "alpha", "beta", "real" };
	private final static String[] REGION_MODES       = { "none", "manually", "automatically" };
	private final static String[] USE_FIXED_PLAYERID = { "true", "false" };

	String zone;
	String gameCode;
	String regionMode;
	boolean useFixedPlayerID = false;
	Activity activity;
	SelectListener selectListener;

	public interface SelectListener
	{
		public void onSelected(String gameCode, String zone, String regionMode, boolean useFixedPlayerID);
	}

	public void select(final Activity activity, final SelectListener listener)
	{
		this.activity = activity;
		selectListener = listener;

		selectGameCode();
	}

	void selectGameCode()
	{
		final AlertDialog.Builder alert = new AlertDialog.Builder(activity);
		alert.setTitle("GameCode");
		alert.setCancelable(false);

		final EditText input = new EditText(activity);
		input.setPadding(20, 20, 20, 20);
		input.setHint("banana");

		alert.setView(input);

		alert.setPositiveButton("Ok", new DialogInterface.OnClickListener()
		{
			public void onClick(DialogInterface dialog, int whichButton)
			{
				gameCode = input.getText().toString().trim();

				if (TextUtils.isEmpty(gameCode))
				{
					gameCode = "banana";
				}

				selectZone();
				dialog.dismiss();
			}
		});

		alert.show();
	}

	void selectZone()
	{
		new AlertDialog.Builder(activity).setTitle("ZONE").setCancelable(false).setSingleChoiceItems(ZONES, 0, null)
			.setPositiveButton(android.R.string.ok, new DialogInterface.OnClickListener()
		{

			@Override public void onClick(DialogInterface dialog, int which)
			{
				dialog.dismiss();

				int selectedPosition = ((AlertDialog)dialog).getListView().getCheckedItemPosition();
				zone = ZONES[selectedPosition];

				selectRegionMode();
				dialog.dismiss();
			}
		}).setCancelable(false).show();
	}

	void selectRegionMode()
	{
		new AlertDialog.Builder(activity).setTitle("REGION MODE").setCancelable(false).setSingleChoiceItems(REGION_MODES, 0, null)
		.setPositiveButton(android.R.string.ok, new DialogInterface.OnClickListener()
		{

			@Override public void onClick(DialogInterface dialog, int which)
			{
				dialog.dismiss();

				int selectedPosition = ((AlertDialog)dialog).getListView().getCheckedItemPosition();
				regionMode = REGION_MODES[selectedPosition];

				selectUseFixedPlayerID();
				dialog.dismiss();
			}
		}).setCancelable(false).show();
	}

	void selectUseFixedPlayerID()
	{
		new AlertDialog.Builder(activity).setTitle("FIXED PLAYERID").setCancelable(false).setSingleChoiceItems(USE_FIXED_PLAYERID, 0, null)
		.setPositiveButton(android.R.string.ok, new DialogInterface.OnClickListener()
		{
			@Override public void onClick(DialogInterface dialog, int which)
			{
				dialog.dismiss();

				int selectedPosition = ((AlertDialog)dialog).getListView().getCheckedItemPosition();

				Log.e("TEST", "useFixedPlayerID : " + USE_FIXED_PLAYERID [selectedPosition]);
				useFixedPlayerID = Boolean.parseBoolean(USE_FIXED_PLAYERID [selectedPosition]);
				Log.e("TEST", "useFixedPlayerID : " + useFixedPlayerID);
				selectListener.onSelected(gameCode, zone, regionMode, useFixedPlayerID);
			}
		}).setCancelable(false).show();
	}
}

